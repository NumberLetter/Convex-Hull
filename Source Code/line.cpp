#include "line.h"

/**
    Samuel Millette
	12.3.2020
	For my CSC212 class take fall of 2020 at the University of Rhode Island
 */

/**
 * This class is used for calculations in 'quickHull.cpp'
 */

//TODO: revise this to remove the closest point, redundant as computers don't need math
//TODO: to be made simpler by using the point closest to the y axis like us monkeys
/**
 * This function calculates the Y intercept of the parent line.
 * This function is only called in the case where we know the two points used for making the line are distinct.
 *  That is, this function is called when the two points will not generate an infinite or illegal line.
 *
 * @param p1: A point
 * @param p2: A point
 * @param slope: A double that represents the slope of the parent line
 * @return: a double representing the y coordinate of the y intercept
 */

double Line::calculateYIntercept(Point p1, Point p2, double slope){
    //attempting
    int p1XAbsoluteVal = abs(p1.getX());
    int p2XAbsoluteVal = abs(p2.getX());

    Point closest = p1;

    if ( p1XAbsoluteVal > p2XAbsoluteVal ){
        closest = p2;
    }
    else{
        closest = p1;
    }

    //from here we have our slope, m, our y value being examined, closest.getY(), and our x value, closest.getX()
    //this is useful because we can plug those into our linear equation and by algebra obtain b, our y intercept

    double intercept = closest.getY() - (slope * closest.getX());
    return intercept;
}

/**
 * constructor that checks for vertical lines
 * important to note that this does not check for like points, that is, this will return Nan for differences
 * of 0 between point coordinates, essentially breaking the rest of the code
 */
Line::Line(Point p1, Point p2){

    this -> p1 = p1;
    this -> p2 = p2;

    //In the case where the slope of the line is not infinite, the two x coordinates are distinct
    if( p2.getX() - p1.getX() != 0 ) {
        // calculate slope normally
        this -> slope = (p2.getY() - p1.getY()) / (p2.getX() - (double)p1.getX());
        this -> intercept = calculateYIntercept(p1, p2, slope);
    }
    //In the case where the two point have the same x values, where the line is vertical
    else {
        this -> slope = (p2.getY() - p1.getY()) / (p2.getX() - (double)p1.getX());

        //key difference here, the intercept variable exists to symbolize the x intercept of the vertical line
        this -> intercept = p1.getX();
    }
}

/**
 * These functions return the initializer points of the line
 * @return a point
 */

Point Line::getP1(){
    return p1;
}

Point Line::getP2(){
    return p2;
}

/**
 * This function is a basic public getter for the line's intercept ( either x or y, see constructor)
 * @return The intercept of the line
 */
double Line::getIntercept(){
    return intercept;
}

/**
 * This function is a basic public getter for the line's slope
 *
 * @return The slope of the line
 */
double Line::getSlope(){
    return slope;
}

/**
 * This function calculates a perpendicular line off of the line this function is invoked upon.
 * Uses the pythagorean theorem and some properties of linear equations to find hypotenuse
 *
 * @param pExamined: a point object that lies somewhere on a coordinate plain, used to calculate distance
 * @return the shortest length between the line and pExamined ( the hypotenuse )
 */
double Line::getDistanceToPoint(Point pExamined){
    //In the case were the slope of this line is vertical, return delta x
    if( std::isinf(slope) ){
        return std::abs(intercept - pExamined.getX());
    }
    else if( slope == 0 ){
        return std::abs(intercept - pExamined.getY());
    }
    //In the case where the line itself is Nan, return Nan
    else if( slope != slope){
        return slope;
    }
    else{
        /**
         * these following lines calculate the x coordinate of the point of interception between
         * the line that this method is being evoked on, and the perpendicular of this line
         * and the point passed into the function. These calculations appear to be mathematically sound
        */
        double perpendicularSlope = -1.0/slope;
        double perpendicularYIntercept = (-perpendicularSlope * pExamined.getX()) + pExamined.getY();
        double xPointOfInterception = (perpendicularYIntercept - intercept) / (slope - perpendicularSlope);

        /**
        * From here we go on to use pythagorean theorem to calculate the actual distance from line to pExamined
        */
        double deltaX = std::abs(xPointOfInterception - pExamined.getX());
        double yPointOfInterception = (slope * xPointOfInterception) + intercept;
        double deltaY = std::abs(yPointOfInterception - pExamined.getY());

        return std::sqrt(std::pow(deltaX, 2.0) + std::pow(deltaY, 2.0));
    }
}

/**
 * This function takes in a point and returns whether the point lies above, below, or on the parent line.
 *
 * @param pExamined: A point
 * @return: 1 where point lies above, 0 where point lies on, and -1 where point lies below
 */

int Line::isPointGreater(Point pExamined){
    if( pExamined.getY() > (slope * pExamined.getX()) + intercept ){
        return 1;
    }
    else if( pExamined.getY() < (slope * pExamined.getX()) + intercept ){
        return -1;
    }
    else{
        return 0;
    }
}