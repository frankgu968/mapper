#include <vector>
#include <cassert>
#include "StreetsDatabaseAPI.h"

#include "path_verify.h"

namespace ece297test {

bool path_is_legal(unsigned start_intersection, unsigned end_intersection, const std::vector<unsigned>& path) {
    //'start_intersection' is the intersection id of the starting intersection
    //'end_intersection' is the intersection id of the ending intersection
    //'path' is a vector street segment id's

    if (path.size() < 1) {
        return false; //If it is a valid path it must contain at-least one segment

    } else {
        //General case
        //To verify the path by walking along each segment checking:
        //  * That we are going a legal direction (i.e. not against one-ways)
        //  * That each segment is connected to the previous intersection
        //  * That the final segment is connected to the end_intersection
        //We start our intersection at the start_intersection
        assert(path.size() >= 1);

        unsigned curr_intersection = start_intersection;
        for (int i = 0; i < (int) path.size(); i++) {
            StreetSegmentInfo seg_info = getStreetSegmentInfo(path[i]);


            //Are we moving forward or back along the segment?
            bool seg_traverse_forward;
            if (seg_info.to == curr_intersection) {
                //Moving backwards
                seg_traverse_forward = false;

            } else if (seg_info.from == curr_intersection) {
                //Moving forwards
                seg_traverse_forward = true;

            } else {
                //This segment isn't connected to the current intersection
                return false;
            }

            //Are we going the wrong way along the segment?
            if (!seg_traverse_forward && seg_info.oneWay) {
                return false;
            }

            //Advance to the next intersection
            curr_intersection = (seg_traverse_forward) ? seg_info.to : seg_info.from;

        }

        //We should be at the end intersection
        if (curr_intersection != end_intersection) {
            return false;
        }

    }

    //Everything validated
    return true;
}

}
