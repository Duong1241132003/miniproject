#ifndef SHUFFLE_MANAGER_H
#define SHUFFLE_MANAGER_H

#include <vector>
#include <set>
#include <random>
#include "Song.h"

/*
 * ShuffleManager
 * --------------
 * Manages shuffled playback without immediate repeats.
 */
class ShuffleManager
{
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
    * Random number generator for shuffle.
    */
    std::mt19937 gen;
    
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
    void printAllSongs() const;
};

#endif
