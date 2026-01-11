#include "ShuffleManager.h"
#include <iostream> 
#include <algorithm>
#include <random>

void ShuffleManager::initialize(const std::vector<Song*>& playlist)
{
    shuffledSongs = playlist;
    playedSongIDs.clear();

     /*
     * Khởi tạo bộ sinh số ngẫu nhiên Mersenne Twister (mt19937)
     * với seed từ random_device để tăng tính ngẫu nhiên.
     */
    std::random_device rd;
    gen = std::mt19937(rd());
}

Song* ShuffleManager::getNextSong()
{
    if (shuffledSongs.empty())
    {
        return nullptr;
    }

    if (playedSongIDs.size() == shuffledSongs.size())
    {
        return nullptr;   /* kết thúc 1 vòng shuffle */
    }

    /*
     * Tạo bộ phân bố đều để sinh chỉ số ngẫu nhiên hợp lệ
     * trong phạm vi [0, shuffledSongs.size() - 1].
     */
    std::uniform_int_distribution<> dist(0, shuffledSongs.size() - 1);

    /*
     * Lặp cho đến khi tìm được một bài hát chưa phát trong chu kỳ hiện tại.
     */
    while (true)
    {
        /*
         * Sinh một chỉ số ngẫu nhiên để chọn bài hát.
         */
        int idx = dist(gen);

         /*
         * Lấy ID của bài hát được chọn.
         * ID được dùng để kiểm tra trùng lặp thông qua set.
         */
        int id = shuffledSongs[idx]->id;

        /*
         * Kiểm tra xem bài hát này đã được phát trong chu kỳ hiện tại chưa.
         */
        if (playedSongIDs.find(id) == playedSongIDs.end())
        {
            playedSongIDs.insert(id);  
            return shuffledSongs[idx];
        }
    }
}

void ShuffleManager::getAllSongs() const
{
    for (const Song* song : shuffledSongs)
    {
       std::cout
        << "ID: " << song -> id
        << " | Title: " << song -> title
        << " | Artist: " << song -> artist
        << " | Album: " << song -> album
        << " | Duration: " << song -> duration << " s"
        << " | Path: " << song -> path
        << '\n';
    }
}