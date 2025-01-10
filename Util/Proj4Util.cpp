#include "Proj4Util.h"
#include "DsUtil/DsMath.h"

#include <proj_api.h>

#include "DsUtil/DsMath.h"
#include "DsCore/DsAssert.h"
#include <QString>

#include <qglobal.h>

#include <iomanip>

namespace DsUtil {

bool Proj4Util::longlatToUtm(double& longitude,double& latitude){
    bool result = longlatToUtm(&longitude,&latitude,1);
    return result;
}


bool Proj4Util::longlatToUtm(double *longitude,double *latitude, int numPoint)
{
    //入力はラジアン
    Q_ASSERT(longitude[0] < DsUtil::DsMath::pi() || longitude[0] > -DsUtil::DsMath::pi());
    Q_ASSERT(latitude[0] < DsUtil::DsMath::pi() / 2.0 || latitude[0] > -DsUtil::DsMath::pi() / 2.0);

    projPJ pj_longlat, pj_utm;

    /*
     * 測地系指定　+datum=WGS84
     *　楕円体指定　+ellps=WGS84
     * ゾーン指定　　+zone=54
     */

    if (!(pj_longlat = pj_init_plus("+proj=longlat +datum=WGS84")) ){
        DS_ERR("pj_init_plus error: longlat");

        return false;
    }

    if (!(pj_utm = pj_init_plus("+proj=utm +zone=54 +datum=WGS84")) ){
        //if (!(pj_utm = pj_init_plus("+init=EPSG:2451")) ){
        DS_ERR("pj_init_plus error: utm");
        return false;
    }

    projPJ src = pj_longlat;
    projPJ dst = pj_utm;

    int p = pj_transform(src , dst, numPoint, 0, longitude, latitude, NULL );

    if(p != 0){
        DS_ERR("Proj4 error code: " << p <<  "Proj4 error message: " << pj_strerrno(p));
        return false;
    }


    return true;
}

bool Proj4Util::convertCoord(double *x,double *y, int numPoint,const char* srcDesc,const char* dstDesc)
{
    if(QString(srcDesc).contains("+proj=longlat") || QString(srcDesc).contains("+init=EPSG:4326")){

        for(int i = 0;i < numPoint;i++){
            //入力はラジアン
            if(! (x[i] < DsUtil::DsMath::pi() || x[0] > -DsUtil::DsMath::pi()) ){
                DS_ERR("invalid longtude.");
                return false;
            }
            if(! (y[i] < DsUtil::DsMath::pi() / 2.0 || y[0] > -DsUtil::DsMath::pi() / 2.0)){
                DS_ERR("invalid latitude.");
                return false;
            }
        }//for
    }

    projPJ src, dst;

    if (!(src = pj_init_plus(srcDesc)) ){
        DS_ERR("pj_init_plus error: src " + std::string(srcDesc));

        return false;
    }

    if (!(dst = pj_init_plus(dstDesc)) ){
        DS_ERR("pj_init_plus error: dst " + std::string(dstDesc));
        return false;
    }


//    DS_DEBUG_VALUE(std::fixed<<std::setprecision(15) << "before " << x[0]);
//    DS_DEBUG_VALUE(std::fixed<<std::setprecision(15) << "before "<< x[1]);

    int p = pj_transform(src , dst, numPoint, 0, x, y, NULL );

//    DS_DEBUG_VALUE(std::fixed<<std::setprecision(15) << "after  " << x[0]);
//    DS_DEBUG_VALUE(std::fixed<<std::setprecision(15) << "after  " << x[1]);

//    double xdebug[2] = {88000.000000000000000,90000.000000000000000};
//    double ydebug[2] = {0,0};

//    p = pj_transform(src , dst, numPoint, 0, xdebug, ydebug, NULL );

//    DS_DEBUG_VALUE(std::fixed<<std::setprecision(15) << xdebug[0]);
//    DS_DEBUG_VALUE(std::fixed<<std::setprecision(15) << xdebug[1]);

//    double xdebug2[3] = {88000.000000000000000,90000.000000000000000,92000.000000000000000};
//    double ydebug2[3] = {0,0,0};

//    p = pj_transform(src , dst, 3, 0, xdebug2, ydebug2, NULL );

//    DS_DEBUG_VALUE(std::fixed<<std::setprecision(15) << xdebug2[0]);
//    DS_DEBUG_VALUE(std::fixed<<std::setprecision(15) << xdebug2[1]);

    if(p != 0){
        DS_ERR("Proj4 error code: " << p <<  "Proj4 error message: " << pj_strerrno(p));
        return false;
    }


    return true;
}

}//ns DsUtil
