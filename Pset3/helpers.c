// Helper functions for music

#include <cs50.h>
#include <string.h>
#include "helpers.h"
#include <math.h>

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    // Parses fraction from string and gets the numerator and denominator as integers
    int numerator = fraction[0] - '0';
    int denominator = fraction[2] - '0';
    int counter = 0;
    // If denominator is 8, it is an 8th note so the duration is given numerator.
    if (denominator == 8)
    {
        return numerator;
    }
    // If denominator is less than 8 (presuming duration will not take in anything greater than 8)
    // I find the duration by multiplying the denominator by 2 until it reaches 8 so we are in the
    // eighths duration and then multiply the numerator by that same amount because math.
    // We now have the duration so we can return this amount (the numerator)
    else
    {
        while (denominator != 8)
        {
            denominator *= 2;
            counter++;
        }
        numerator *= counter * 2;
        return numerator;
    }
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    // Initialize scale and the corresponding positions of each key on the leftmost side of piano
    string scale[] = {"A", "A#", "Bb", "B", "C", "C#", "Db", "D", "D#", "Eb", "E", "F", "F#", "Gb", "G", "G#", "Ab"};
    int startPos[] = {0, 1, 1, 2, 3, 4, 4, 5, 6, 6, 7, 8, 9, 9, 10, 11, 11};
    // Find if note is a sharp/flat or not
    bool blackkey = (strlen(note) == 3) ? true : false;
    // Copy information about note and octave
    char noteKey[3];
    int o = 0;
    if (blackkey)
    {
        strncpy(noteKey, note, 2);
        noteKey[2] = '\0';
        o = note[2] - '0';
    }
    else
    {
        strncpy(noteKey, note, 1);
        noteKey[1] = '\0';
        o = note[1] - '0';
    }
    // Initialize variable for A4 position
    int a = 48;
    // Find position of note on keyboard. I do this by finding what octave the key is in, multiplying that by
    // the number of keys in an octave, and then adding the first position or occurence of that key.
    int notePos = 0;
    for (int i = 0; i < sizeof(scale) / sizeof(scale[0]); i++)
    {
        if (strcmp(noteKey, scale[i]) == 0)
        {
            notePos = (i <= 3) ? o * 12 + startPos[i] : o * 12 + startPos[i] - 12;
            break;
        }
    }
    // Find amount of semitones, n. If notePos is less than position of A4, n must be negative
    int n = (notePos > a) ? notePos - a : -(a - notePos);
    // Find power to multiply 2 by
    float p = (float) n / (float) 12;
    // Find frequency
    int f = round(pow(2, (p)) * 440);
    return f;
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    // If user hits enter, they are inputting a rest. Return true
    if (strcmp(s, "") == 0)
    {
        return true;
    }
    // Otherwise, it is not a rest. Return false
    else
    {
        return false;
    }
}
