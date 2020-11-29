#include "quickHull.h"
#include "convexFileHandling.h"

//TODO: ask about defining these global variables in a header file, benefits, keywords, etc.
Point minX(100,100);
Point maxX(-1,-1);
std::vector< Point > disregardedPoints;

std::vector< Point > quickHullDivide(std::vector < Point > setOfPoints, Line dividingSegment){
    std::vector< Point > hull;

    //Base case, if there are no points to be considered
    if( setOfPoints.empty() ){
        return hull;
    }

    //Initialize a point object that will store point furthest from the dividing segment
    Point furthest;
    int distance = 0;

    //Find point furthest from line segment
    for( int i = 0; i < setOfPoints.size(); i++ ){
        if( std::abs(dividingSegment.getDistanceToPoint(setOfPoints[i])) > distance ){
            distance = std::abs(dividingSegment.getDistanceToPoint(setOfPoints[i]));
            furthest = setOfPoints[i];
        }
    }

    //In the case where there are no points that lie away from the dividing line segment, distance remains unchanged
    //Return nothing, this section of the hull is already encircled by the dividing line segment
    if( distance == 0 ){
        return hull;
    }

    //Bool to store if the following calculations will take place above or below the dividing line segment
    bool aboveDividingSegment = (dividingSegment.isPointGreater(furthest) == 1) ? true : false;

    Line leftLine( dividingSegment.getP1(), furthest );
    std::vector< Point > leftPoints;
    Line rightLine( furthest, dividingSegment.getP2() );
    std::vector< Point > rightPoints;

    /**
     * The following comparisons use XNOR, the gate is true if and only if both inputs are the same
     * This makes sense because if comparisons are being done negative relative to the dividing segment,
     * then checking if a point lies above one of the new segments would not be useful in seeing if the
     * point lies outside of the generated triangle.
     */

    for( int i = 0; i < setOfPoints.size(); i++ ){
        //If the comparison orientation is the same as where the point lies, add point to left array
        if( aboveDividingSegment == ( leftLine.isPointGreater(setOfPoints[i] ) == 1) ){
            leftPoints.push_back(setOfPoints[i]);
        }
        //If same as above, add point to right array
        else if( aboveDividingSegment == ( rightLine.isPointGreater(setOfPoints[i]) == 1) ){
            rightPoints.push_back(setOfPoints[i]);
        }
        //If the point lies neither outside the right line nor outside the left, add to discard array
        else if( setOfPoints[i].getX() != furthest.getX() && setOfPoints[i].getY() != furthest.getY() ){
            disregardedPoints.push_back(setOfPoints[i]);
        }
    }

    if( aboveDividingSegment ) {
        //Stores the output of the recursive quickHullDivide call with left array and inserts that into the hull vector
        std::vector <Point> leftHull = quickHullDivide(leftPoints, leftLine);
        hull.insert(hull.end(), leftHull.begin(), leftHull.end());

        hull.push_back(furthest);

        //Stores the output of the recursive quickHullDivide call with right array and inserts that into the hull vector
        std::vector <Point> rightHull = quickHullDivide(rightPoints, rightLine);
        hull.insert(hull.end(), rightHull.begin(), rightHull.end());
    }
    else{
        //Stores the output of the recursive quickHullDivide call with right array and inserts that into the hull vector
        std::vector <Point> rightHull = quickHullDivide(rightPoints, rightLine);
        hull.insert(hull.end(), rightHull.begin(), rightHull.end());

        hull.push_back(furthest);

        //Stores the output of the recursive quickHullDivide call with left array and inserts that into the hull vector
        std::vector <Point> leftHull = quickHullDivide(leftPoints, leftLine);
        hull.insert(hull.end(), leftHull.begin(), leftHull.end());
    }

    return hull;
}

std::vector< Point > quickHullBase(std::vector< Point > allPoints){
    //Complete hull vector
    std::vector< Point > hull;

    //Line do do point comparisons,
    Line minMaxLine(minX,maxX);

    //Sets of points to pass into the divide and conquer algorithm baby
    std::vector< Point > upperSet;
    std::vector< Point > lowerSet;

    //If the point lies above minmax line, add to upper, if it lies below, add to lower. Disregards minX and maxX
    for( int i = 0; i < allPoints.size(); i++ ){
        if( minMaxLine.isPointGreater(allPoints[i]) == 1 ){
            upperSet.push_back(allPoints[i]);
        }
        else if( minMaxLine.isPointGreater(allPoints[i]) == -1){
            lowerSet.push_back(allPoints[i]);
        }
    }

    //Since minX is already part of the hull by definition, add to final hull
    hull.push_back(minX);

    //Stores the output of the quickHullDivide with upper set and inserts that into the hull vector
    std::vector< Point > upperHull = quickHullDivide(upperSet, minMaxLine);
    hull.insert( hull.end(), upperHull.begin(), upperHull.end() );

    //Add a single value to hull
    hull.push_back(maxX);

    //Stores the output of the quickHullDivide with lower set and inserts that into the hull vector
    std::vector< Point > lowerHull = quickHullDivide(lowerSet, minMaxLine);
    hull.insert( hull.end(), lowerHull.begin(), lowerHull.end() );

    return hull;
}

void quickHull( std::string inFilePath ){
    std::vector< Point > allPoints = csvToVector(inFilePath, minX, maxX);

    std::vector< Point > hulledPoints = quickHullBase(allPoints);

    hullVectorToCSV("quickhullPairs.csv", hulledPoints, disregardedPoints);
}