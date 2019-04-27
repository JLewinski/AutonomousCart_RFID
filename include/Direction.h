#pragma once
/** File: Direction.h
 * Author: Jacob Lewinski (jdlewinski)
 * Description: Contains a reference to all the possible
 * directions including STOPPED. This is helpful to allow
 * for all modules to use the same strongly typed directions. 
 */
enum Direction
{
    North,
    East,
    South,
    West,
    Other,
    Stopped
};
