#include <iostream>
#include <map>
#include <optional>
#include <ranges>
#include <vector>

using namespace std;

using FileID = int;

void compact1(vector<optional<FileID>>& blocks)
{
    auto r = blocks.size();
    for (auto l = 0; l < r; ++l)
    {
        if (blocks[l])
            continue;
        do
        {
            --r;
        } while (!blocks[r]);
        if (l < r)
        {
            blocks[l] = blocks[r];
            blocks[r] = nullopt;
        }
    }
}

struct File
{
    size_t start;
    size_t const size;
    optional<size_t> pregap;
    optional<size_t> postgap;
};

vector<optional<FileID>> compact2(map<FileID, File> files, map<size_t, size_t> gaps)
{
    auto const& last_file = files.rbegin()->second;
    size_t num_blocks = last_file.start + last_file.size;

    for (auto& [file_id, file] : views::reverse(files))
    {
        optional<pair<size_t, size_t>> found_gap;
        for (auto [gap_start, gap_size] : gaps)
        {
            if (file.start < gap_start)
                break;
            if (file.size > gap_size)
                continue;
            found_gap = {gap_start, gap_size};
            break;
        }
        if (!found_gap)
            continue;
        auto [gap_start, gap_size] = *found_gap;
        size_t newgapsize = file.size;
        if (file.postgap)
        {
            newgapsize += gaps[*file.postgap];
            gaps.erase(*file.postgap);
        }
        if (file.pregap)
            gaps[*file.pregap] += newgapsize;
        else
            gaps[file.start] = newgapsize;
        gaps.erase(gap_start);
        file.start = gap_start;
        if (file.size < gap_size)
            gaps[gap_start + file.size] = gap_size - file.size;
    }

    vector<optional<FileID>> blocks (num_blocks);
    for (auto const& [file_id, file] : files)
    {
        for (size_t i = file.start; i < file.start + file.size; ++i)
            blocks[i] = file_id;
    }
    return blocks;
}

uint64_t checksum(vector<optional<int>> const& blocks)
{
    uint64_t checksum = 0;
    for (auto [pos, id] : views::enumerate(blocks))
    {
        if (id)
            checksum += pos * *id;
    }
    return checksum;
}

int main()
{
    string disk_map;
    getline(cin, disk_map);

    vector<optional<FileID>> blocks;
    map<FileID, File> files;
    map<size_t, size_t> gaps;
    optional<size_t> last_gap;
    File* last_file = nullptr;
    for (auto [i, digit] : views::enumerate(disk_map))
    {
        optional<FileID> file_id = i % 2 ? nullopt : make_optional(i / 2);
        size_t start = blocks.size();
        size_t size = digit - '0';
        if (file_id)
        {
            files.emplace(make_pair(*file_id, File{start, size, last_gap, nullopt}));
            last_file = &files.at(*file_id);
        }
        else if (size)
        {
            gaps[start] = size;
            last_gap = start;
            if (last_file)
                last_file->postgap = start;
        }
        else
            last_gap = nullopt;
        for (int j = 0; j < size; ++j)
        {
            blocks.push_back(file_id);
        }
    }
    compact1(blocks);
    cout << checksum(blocks) << '\n';

    auto blocks2 = compact2(files, gaps);
    cout << checksum(blocks2) << '\n';
    return 0;
}
