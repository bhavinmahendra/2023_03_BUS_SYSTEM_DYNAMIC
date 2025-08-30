#ifndef _MAIN_DYNAMIC_
#define _MAIN_DYNAMIC_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_CHAR 65535
#define TRUE 1
#define FALSE 0

struct routesOnly {
    char *route;
    struct routesOnly *prev, *next;
};
typedef struct routesOnly *linkRoutesOnly;
typedef struct routesOnly RoutesOnly;

struct stopsOnly {
    char *stop;
    double costLocal, durationLocal;
    struct stopsOnly *prev, *next;
};
typedef struct stopsOnly *linkStopsOnly;
typedef struct stopsOnly StopsOnly;

struct route {
    char *name;
    double costTotal, durationTotal;
    linkStopsOnly headStopsOnly, stopsAll;
    struct route *prev, *next;
};
typedef struct route Route;
typedef struct route *linkRoute;

struct stop {
    char *name;
    double latitude, longitude;
    linkRoutesOnly headRoutesOnly, routesAll;
    struct stop *prev, *next;
};
typedef struct stop Stop;
typedef struct stop *linkStop;

/* Auxiliary funtions */
int size(char **lineList);
char **stringToArray(char *line);
void bubbleSort(linkRoutesOnly myList);

/* l command auxiliary functions */
linkRoute addRoute(char *name);
linkStop addStop(char *name, double latitude, double longitude);
linkRoutesOnly addRouteToStop(linkStop stop, char *routeName);
linkStopsOnly addStopToRoute(linkRoute route, char *stopName, double cost, double duration);
linkStopsOnly insertStopBefore(linkRoute route, char *stopName, double cost, double duration);
linkStopsOnly insertStopAfter(linkRoute route, char *stopName, double cost, double duration);

/* Manipulation of the program */
void cCommandNoArgs(linkRoute headRoute);
linkRoute cCommandOneArgs(linkRoute headRoute, char *name);
void cCommandTwoArgs(linkRoute headRoute, char *name, char *option);
void pCommandNoArgs(linkStop headStop);
void pCommandOneArgs(linkStop headStop, char *myStop);
linkStop pCommandThreeArgs(linkStop headStop, char* name, double latitude, double longitude);
void lCommandFiveArgs(linkStop *headStop, linkRoute *headRoute, char *routeName,
                      char *fromName, char *toName, double cost, double duration);
void iCommandNoArgs(linkStop headStop);
void rCommandOneArgs(linkRoute *headRoute, linkStop *headStop, char *name);
void eCommandOneArgs(linkRoute *headRoute, linkStop *headStop, char *name);
void aCommandNoArgs(linkRoute *headRoute, linkStop *headStop);

#endif
