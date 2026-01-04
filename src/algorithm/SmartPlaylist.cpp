#include "SmartPlaylist.h"
#include <queue>
#include <set>

/*
 * Generates a smart playlist based on artist and album similarity.
 */
PlaybackQueue generateSmartPlaylist(
    Song& startSong,
    MusicLibrary& library,
    int maxSize
)
{
    PlaybackQueue resultQueue;

    /*
     * Queue used for BFS traversal.
     */
    std::queue<const Song*> bfsQueue;

    /*
     * Set to track songs already added to playlist.
     */
    std::set<int> visitedSongIDs;

    /*
     * Initialize BFS with the starting song.
     */
    bfsQueue.push(&startSong);
    visitedSongIDs.insert(startSong.id);
    resultQueue.addSong(startSong);

    /*
     * Perform BFS until playlist reaches max size.
     */
    while (!bfsQueue.empty() && visitedSongIDs.size() < static_cast<size_t>(maxSize))
    {
        const Song* currentSong = bfsQueue.front();
        bfsQueue.pop();

        /*
         * Explore neighbors by artist.
         */
        auto artistNeighbors = library.findSongsByArtist(currentSong->artist);

        for (const Song* neighbor : artistNeighbors)
        {
            if (visitedSongIDs.size() >= static_cast<size_t>(maxSize))
            {
                break;
            }

            if (visitedSongIDs.insert(neighbor->id).second)
            {
                resultQueue.addSong(*neighbor);
                bfsQueue.push(neighbor);
            }
        }

        /*
         * Explore neighbors by album.
         */
        auto albumNeighbors = library.findSongsByAlbum(currentSong->album);

        for (const Song* neighbor : albumNeighbors)
        {
            if (visitedSongIDs.size() >= static_cast<size_t>(maxSize))
            {
                break;
            }

            if (visitedSongIDs.insert(neighbor->id).second)
            {
                resultQueue.addSong(*neighbor);
                bfsQueue.push(neighbor);
            }
        }
    }

    return resultQueue;
}
