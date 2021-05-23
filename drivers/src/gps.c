#include "gps.h"


void gps_init()
{
    uart0_init();
}

double distance_sphere(geographic_point_t *p1, geographic_point_t *p2)
{
    // Based on Haversine formula & Great-circle distance
    // https://en.wikipedia.org/wiki/Haversine_formula#Formulation
    // https://en.wikipedia.org/wiki/Great-circle_distance

    // Convert lat/long to radians
    double lat1 = p1->lat * (PI / 180.0);
    double lat2 = p2->lat * (PI / 180.0);
    double lon1 = p1->lon * (PI / 180.0);
    double lon2 = p2->lon * (PI / 180.0);

    double r = 6371009.0;

    double dlat = (lat2 - lat1) / 2.0;
    double dlon = (lon2 - lon1) / 2.0;

    double a = sin(dlat) * sin(dlat) + cos(lat1) * cos(lat2) * sin(dlon) * sin(dlon);
    double distance = 2 * r * asin(sqrt(a));
    return distance;
}

double distance_spheroid(geographic_point_t *p1, geographic_point_t *p2)
{
    // Based on Vincenty's formulae using Inverse problem
    // https://en.wikipedia.org/wiki/Vincenty%27s_formulae

    int MAXITERS = 20;
    // Convert lat/long to radians
    double lat1 = p1->lat * (PI / 180.0);
    double lat2 = p2->lat * (PI / 180.0);
    double lon1 = p1->lon * (PI / 180.0);
    double lon2 = p2->lon * (PI / 180.0);

    double a = 6378137.0;      // WGS84 major axis
    double b = 6356752.314245; // WGS84 semi-major axis
    double f = (a - b) / a;    // flattening of the ellipsoid

    double L = lon2 - lon1;
    double U1 = atan((1.0 - f) * tan(lat1)); // reduced latitude (latitude on the auxiliary sphere)
    double U2 = atan((1.0 - f) * tan(lat2));

    double cosU1 = cos(U1);
    double cosU2 = cos(U2);
    double sinU1 = sin(U1);
    double sinU2 = sin(U2);
    double cosU1cosU2 = cosU1 * cosU2;
    double sinU1sinU2 = sinU1 * sinU2;

    double sigma = 0.0; // angular separation between points
    double cosSqAlpha = 0.0;
    double cos2SM = 0.0;
    double cosSigma = 0.0;
    double sinSigma = 0.0;
    double cosLambda = 0.0;
    double sinLambda = 0.0;

    double lambdaOrig = 0.0;
    double t1 = 0.0;
    double t2 = 0.0;
    double sinSqSigma = 0.0;
    double sinAlpha = 0.0;

    double delta = 0.0;
    double C = 0.0;

    double lambda = L; // initial guess
    for (int iter = 0; iter < MAXITERS; iter++)
    {
        lambdaOrig = lambda;
        sinLambda = sin(lambda);
        cosLambda = cos(lambda);
        t1 = cosU2 * sinLambda;
        t2 = cosU1 * sinU2 - sinU1 * cosU2 * cosLambda;
        sinSqSigma = t1 * t1 + t2 * t2;
        sinSigma = sqrt(sinSqSigma);                                          // (1)
        cosSigma = sinU1sinU2 + cosU1cosU2 * cosLambda;                       // (2)
        sigma = atan2(sinSigma, cosSigma);                                    // (3)
        sinAlpha = (sinSigma == 0) ? 0.0 : cosU1cosU2 * sinLambda / sinSigma; // (4)
        cosSqAlpha = 1.0 - sinAlpha * sinAlpha;
        cos2SM = (cosSqAlpha == 0) ? 0.0 : cosSigma - 2.0 * sinU1sinU2 / cosSqAlpha; // (5)

        C = (f / 16.0) *
            cosSqAlpha *
            (4.0 + f * (4.0 - 3.0 * cosSqAlpha)); // (6)

        lambda = L +
                 (1.0 - C) * f * sinAlpha *
                     (sigma + C * sinSigma *
                                  (cos2SM + C * cosSigma *
                                                (-1.0 + 2.0 * cos2SM * cos2SM))); // (7)

        delta = (lambda - lambdaOrig) / lambda;
        if (abs(delta) < 1.0e-12)
            break;
    }

    double uSquared = cosSqAlpha * ((a * a - b * b) / (b * b)); // (8)
    double A = 1 + (uSquared / 16384.0) *
                       (4096.0 + uSquared *
                                     (-768 + uSquared * (320.0 - 175.0 * uSquared))); // (9)
    double B = (uSquared / 1024.0) *
               (256.0 + uSquared *
                            (-128.0 + uSquared * (74.0 - 47.0 * uSquared))); // (10)
    double cos2SMSq = cos2SM * cos2SM;
    double deltaSigma = B * sinSigma *
                        (cos2SM + (B / 4.0) *
                                      (cosSigma * (-1.0 + 2.0 * cos2SMSq) -
                                       (B / 6.0) * cos2SM *
                                           (-3.0 + 4.0 * sinSigma * sinSigma) *
                                           (-3.0 + 4.0 * cos2SMSq))); // (11)

    double distance = b * A * (sigma - deltaSigma); // (12)
    return distance;
}
