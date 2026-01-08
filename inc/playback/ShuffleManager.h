#ifndef SHUFFLE_MANAGER_H
#define SHUFFLE_MANAGER_H

#include <vector>
#include <set>
#include "Song.h"

/*
 * ShuffleManager
 * --------------
 * Manages shuffled playback without immediate repeats.
 */
class ShuffleManager
{
public:
    /*
     * Initializes shuffle with a list of songs.
     */
    void initialize(const std::vector<Song*>& playlist);

    /*
     * Returns the next shuffled song.
     * Automatically resets when all songs are played.
     */
    Song* getNextSong();

    /*
     * Print all songs in the shuffled list
     */
    void getAllSongs() const;

private:
    /*
     * Shuffled list of songs.
     */
    std::vector<Song*> shuffledSongs;

    /*
     * IDs of songs already played in current shuffle cycle.
     */
    std::set<int> playedSongIDs;

    /*
     * Current index in shuffled list.
     */
    size_t currentIndex {0};

    /*
     * Resets shuffle cycle.
     */
    void resetCycle();
};

#endif
