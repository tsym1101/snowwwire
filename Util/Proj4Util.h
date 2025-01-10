#ifndef DS_PROJ4UTIL_H
#define DS_PROJ4UTIL_H

#include "DsUtil/DsUtilAPI.h"

namespace DsUtil {
class DSUTILSHARED_EXPORT Proj4Util {
public:
    static bool  longlatToUtm(double& longitude,double& latitude);  //1point
    static bool  longlatToUtm(double *longitude,double *latitude, int numPoint);

    //汎用式
    static bool  convertCoord(double *x, double *y, int numPoint,
                              const char *srcDesc = "+proj=longlat +datum=WGS84",
                              const char *dstDesc = "+proj=utm +zone=54 +datum=WGS84");

};
}//ns DsUtil


#endif // DS_PROJ4UTIL_H


/*
 * 座標系指定 +proj=longlat  eg. longlat(緯度経度) utm(UTM座標) tmerc(平面直角座標)
 * 測地系指定　+datum=WGS84
 *　楕円体指定　+ellps=WGS84
 * ゾーン指定　　+zone=54
 *
 *　EPSGコードで初期化　+init=EPSG:2451
 *　日本地図の平面直角座標はEPSGコードによりプリセット化されているので便利
 *
 *************************************************************
 * 系	EPSG	区域
 * 1	2443	長崎県、鹿児島県の一部
 * 2	2444	福岡県、佐賀県、熊本県、大分県、宮崎県、鹿児島県の一部
 * 3	2445	山口県、島根県、広島県
 * 4	2446	香川県、愛媛県、徳島県、高知県
 * 5	2447	兵庫県、鳥取県、岡山県
 * 6	2448	京都府、大阪府、福井県、滋賀県、三重県、奈良県、和歌山県
 * 7	2449	石川県、富山県、岐阜県、愛知県
 * 8	2450	新潟県、長野県、山梨県、静岡県
 * 9	2451	東京都の一部、福島県、栃木県、茨城県、埼玉県、千葉県、群馬県、神祭川県
 * 10	2452	青森県、秋田県、山形県、岩手県、宮城県
 * 11	2453	北海道の一部
 * 12	2454	北海道の一部
 * 13	2455	北海道の一部
 * 14	2456	東京都の一部
 * 15	2457	沖縄県の一部
 * 16	2458	沖縄県の一部
 * 17	2459	沖縄県の一部
 * 18	2460	東京都の一部
 * 19	2461	東京都の一部
 *************************************************************
 */

