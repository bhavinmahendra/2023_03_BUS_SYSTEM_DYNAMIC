/*
 * Author: Bhavin Mahendra Gulab
 * Description: Program that manipulates a bus route system
 * Dynamic Memory Allocation Version
 * English Version
*/

#include "main.h"

/* Returns the size of an array (of strings) */
int size(char **lineList) {
    int i;
    for (i = 0; lineList[i] != NULL; i++);
    return i;
}

/* Converts the line of the standard input in an array */
char **stringToArray(char *line) {
    char **lineList = malloc(MAX_CHAR * sizeof(char*)), word[MAX_CHAR] = "";
    int i, j = 0, k = 0, quotes = 0, space = 0;

    for (i = 0; line[i] != '\0' && line[i] != '\n'; i++)
        if (line[i] == '"') quotes++;
        else if (line[i] == '\t' || line[i] == ' ') {
            if (quotes % 2 == 0 && space == 0) {
                word[k++] = '\0';
                lineList[j] = malloc(sizeof(char) * (strlen(word) + 1));
                strcpy(lineList[j], word);
                k = 0;
                j++;
                if (quotes == 0)
                    space = 1;
            }
            else if (quotes % 2 == 1)
            word[k++] = line[i];
        }
        else {
            space = 0;
            word[k++] = line[i];
        }

    word[k] = '\0';
    lineList[j] = malloc(sizeof(char) * (strlen(word) + 1));
    strcpy(lineList[j], word);
    lineList[++j] = NULL;

    return lineList;
}

/* Bubble sort algorithm for strings */
void bubbleSort(linkRoutesOnly myList) {
    char *word;
    linkRoutesOnly list1, list2;
    
    for (list1 = myList; list1 != NULL && list1->next != NULL; list1 = list1->next)
        for (list2 = myList; list2->next != NULL; list2 = list2->next)
            if (strcmp(list2->route, list2->next->route) > 0) {
                word = malloc(strlen(list2->route) + 1);
                strcpy(word, list2->route);
                list2->route = list2->next->route;
                list2->next->route = word;
            }
}

/* Adds a route to the list */
linkRoute addRoute(char *name) {
    linkRoute newRoute = malloc(sizeof(Route));
    newRoute->name = malloc(strlen(name) + 1);
    strcpy(newRoute->name, name);

    newRoute->headStopsOnly = malloc(sizeof(StopsOnly));
    newRoute->headStopsOnly->stop = NULL;
    newRoute->headStopsOnly->costLocal = 0.00;
    newRoute->headStopsOnly->durationLocal = 0.00;
    newRoute->headStopsOnly->prev = NULL;
    newRoute->headStopsOnly->next = NULL;

    newRoute->stopsAll = newRoute->headStopsOnly;
    newRoute->costTotal = 0.00;
    newRoute->durationTotal = 0.00;
    newRoute->next = NULL;
    newRoute->prev = NULL;

    return newRoute;
}

/* Adds a stop to the list */
linkStop addStop(char *name, double latitude, double longitude) {
    linkStop newStop = malloc(sizeof(Stop));
    newStop->name = malloc(strlen(name) + 1);
    strcpy(newStop->name, name);
    newStop->latitude = latitude;
    newStop->longitude = longitude;

    newStop->headRoutesOnly = malloc(sizeof(RoutesOnly));
    newStop->headRoutesOnly->route = NULL;
    newStop->headRoutesOnly->prev = NULL;
    newStop->headRoutesOnly->next = NULL;

    newStop->routesAll = newStop->headRoutesOnly;
    newStop->next = NULL;
    newStop->prev = NULL;
    return newStop;
}

/* Adds a route to list of routes of a certain stop */
linkRoutesOnly addRouteToStop(linkStop stop, char *routeName) {
    linkRoutesOnly node, newNode;
    node = stop->headRoutesOnly;
    
    if (node == NULL) {
        node = malloc(sizeof(RoutesOnly));
        node->route = malloc(strlen(routeName) + 1);
        strcpy(node->route, routeName);
        node->next = NULL;
        node->prev = NULL;
        stop->headRoutesOnly = node;
        return node;
    }

    if (node->route == NULL) {
        node->route = malloc(strlen(routeName) + 1);
        strcpy(node->route, routeName);
        return node;
    }

    newNode = malloc(sizeof(RoutesOnly));
    newNode->route = malloc(strlen(routeName) + 1);
    strcpy(newNode->route, routeName);
    newNode->next = NULL;
    newNode->prev = NULL;

    for (node = stop->headRoutesOnly; node->next != NULL; node = node->next);
    node->next = newNode;
    newNode->prev = node;

    return stop->headRoutesOnly;
}

/* Adds a stop to list of stop of a certain route */
linkStopsOnly addStopToRoute(linkRoute route, char *stopName, double cost, double duration) {
    linkStopsOnly node, newNode;
    node = route->headStopsOnly;
    
    if (node->stop == NULL) {
        node->stop = malloc(strlen(stopName) + 1);
        strcpy(node->stop, stopName);
        node->costLocal = cost;
        node->durationLocal = duration;
        return node;
    }

    newNode = malloc(sizeof(StopsOnly));
    newNode->stop = malloc(strlen(stopName) + 1);
    strcpy(newNode->stop, stopName);
    newNode->costLocal = cost;
    newNode->durationLocal = duration;
    newNode->next = NULL;
    newNode->prev = NULL;

    for (node = route->headStopsOnly; node->next != NULL; node = node->next);
    node->next = newNode;
    newNode->prev = node;

    return route->headStopsOnly;
}

/* Auxiliary function for l command, inserts a stop after in the list of stops of a route - before */
linkStopsOnly insertStopBefore(linkRoute route, char *stopName, double cost, double duration) {
    linkStopsOnly node, newNode;
    
    newNode = malloc(sizeof(StopsOnly));
    newNode->stop = malloc(strlen(stopName) + 1);
    strcpy(newNode->stop, stopName);
    newNode->costLocal = cost;
    newNode->durationLocal = duration;
    newNode->prev = NULL;
    newNode->next = NULL;
    
    if (route == NULL)
        return newNode;
    
    node = route->headStopsOnly;
    newNode->prev = node->prev;
    newNode->next = node;
    
    if (node->prev != NULL)
        node->prev->next = newNode;
    node->prev = newNode;
    
    if (route->headStopsOnly == node)
        route->headStopsOnly = newNode;

    return route->headStopsOnly;
}

/* Auxiliary function for l command, inserts a stop after in the list of stops of a route - after */
linkStopsOnly insertStopAfter(linkRoute route, char *stopName, double cost, double duration) {
    linkStopsOnly node, newNode;

    newNode = malloc(sizeof(StopsOnly));
    newNode->stop = malloc(strlen(stopName) + 1);
    strcpy(newNode->stop, stopName);
    newNode->costLocal = cost;
    newNode->durationLocal = duration;
    newNode->next = NULL;
    newNode->prev = NULL;
    
    if (route == NULL)
        return newNode;
    
    for (node = route->headStopsOnly; node->next != NULL; node = node->next);

    node->next = newNode;
    newNode->prev = node;
    
    return route->headStopsOnly;
}

/* Runs the command c to list all routes and its details */
void cCommandNoArgs(linkRoute headRoute) {
    int i;
    linkRoute route;
    linkStopsOnly stops;
    for (route = headRoute; route != NULL; route = route->next) {
        for (i = 0, stops = route->headStopsOnly; stops != NULL && stops->next != NULL;
             i++, stops = stops->next);
        if (i > 0)
            i++;
        if (stops == NULL || route->headStopsOnly->stop == NULL)
            printf("%s %d %.2f %.2f\n", route->name, i, route->costTotal, route->durationTotal);
        else {
            printf("%s %s %s %d %.2f %.2f\n", route->name, route->headStopsOnly->stop,
                    stops->stop, i, route->costTotal, route->durationTotal);
        }
    }
}

/* Runs the command c to add a route or to list the stops of an existent route */
linkRoute cCommandOneArgs(linkRoute headRoute, char *name) {
    int counter;
    linkRoute route, newRoute;
    if (headRoute == NULL)
        return addRoute(name);
    
    for (route = headRoute; route != NULL; route = route->next) {
        counter = 0;
        if (strcmp(route->name, name) == 0) {
            route->stopsAll = route->headStopsOnly;
            if (route->headStopsOnly != NULL && route->headStopsOnly->stop != NULL) {
                printf("%s", route->headStopsOnly->stop);
                counter++;
            }
            if (route->stopsAll != NULL && route->stopsAll->next != NULL)
                for (route->stopsAll = route->stopsAll->next; route->stopsAll != NULL;
                    counter++, route->stopsAll = route->stopsAll->next)
                    printf(", %s", route->stopsAll->stop);
            if (counter > 0)
                putchar('\n');
            break;
        }
        if (route->next == NULL) {
            newRoute = addRoute(name);
            route->next = newRoute;
            newRoute->prev = route;
            break;
        }
    }
    return headRoute;
}

/* Runs the command c to list the stops of an existent route in reverse order */
void cCommandTwoArgs(linkRoute headRoute, char *name, char *option) {
    linkRoute route;
    char op1[] = "i", op2[] = "in", op3[] = "inv", op4[] = "inve", op5[] = "inverso";
    
    if (strcmp(option, op1) != 0 && strcmp(option, op2) != 0 && strcmp(option, op3) != 0 &&
        strcmp(option, op4) != 0 && strcmp(option, op5) != 0) {
        puts("incorrect sort option.");
        return;
    }
    
    for (route = headRoute; route != NULL; route = route->next)
        if (strcmp(route->name, name) == 0)
            break;

    
    for (route->stopsAll = route->headStopsOnly; route->stopsAll->next != NULL;
         route->stopsAll = route->stopsAll->next);
    printf("%s", route->stopsAll->stop);
    

    for (route->stopsAll = route->stopsAll->prev; route->stopsAll != route->headStopsOnly;
         route->stopsAll = route->stopsAll->prev)
        printf(", %s", route->stopsAll->stop);
    
    printf(", %s\n", route->stopsAll->stop);
}

/* Runs the command p to read all stops */
void pCommandNoArgs(linkStop headStop) {
    int i;
    linkStop stop;

    for (stop = headStop; stop != NULL; stop = stop->next) {
        i = 0;
        for (stop->routesAll = stop->headRoutesOnly; stop->routesAll != NULL;
             stop->routesAll = stop->routesAll->next)
            if (stop->routesAll->route)
                i++;
        printf("%s: %16.12f %16.12f %d\n", stop-> name, stop->latitude, stop->longitude, i);
    }
}

/* Runs the command p to read a certain stop */
void pCommandOneArgs(linkStop headStop, char *myStop) {
    linkStop stop;

    for (stop = headStop; stop != NULL; stop = stop->next)
        if (strcmp(stop->name, myStop) == 0) {
            printf("%16.12f %16.12f\n", stop->latitude, stop->longitude);
            return;
        }
    printf("%s: no such stop.\n", myStop);
}

/* Runs the command p to add a stop */
linkStop pCommandThreeArgs(linkStop headStop, char* name, double latitude, double longitude) {
    linkStop stop, newStop;

    if (headStop == NULL)
        return addStop(name, latitude, longitude);

    for (stop = headStop; stop != NULL; stop = stop->next) {
        if (strcmp(stop->name, name) == 0) {
            printf("%s: stop already exists.\n", name);
            break;
        }
        if (stop->next == NULL) {
            newStop = addStop(name, latitude, longitude);
            stop->next = newStop;
            newStop->prev = stop;
            break;
        }
    }
    return headStop;
}

/* Adds a link from fromName to toName in a route */
void lCommandFiveArgs(linkStop *headStop, linkRoute *headRoute, char *routeName, char *fromName,
                      char *toName, double cost, double duration) {
    linkStop stopFrom, stopTo;
    linkRoute route;
    linkRoutesOnly routesOnly;
    linkStopsOnly stopsOnly;
    char *firstName, *lastName;
    
    for (stopFrom = *headStop; stopFrom != NULL && strcmp(stopFrom->name, fromName) != 0;
         stopFrom = stopFrom->next);
    for (stopTo = *headStop; stopTo != NULL && strcmp(stopTo->name, toName) != 0;
         stopTo = stopTo->next);
    for (route = *headRoute; route != NULL && strcmp(route->name, routeName) != 0;
         route = route->next);

    
    if (stopFrom == NULL) {
        printf("%s: no such stop.\n", fromName);
        return;
    }
    if (stopTo == NULL) {
        printf("%s: no such stop.\n", toName);
        return;
    }
    if (route == NULL) {
        printf("%s: no such line.\n", routeName);
        return;
    }
    if (cost < 0 || duration < 0) {
        puts("negative cost or duration.");
        return;
    }
    
    if (route->headStopsOnly != NULL && route->headStopsOnly->stop != NULL) {
        firstName = malloc(strlen(route->headStopsOnly->stop) + 1);
        strcpy(firstName, route->headStopsOnly->stop);
    }
    
    for (stopsOnly = route->headStopsOnly; stopsOnly != NULL && stopsOnly->next != NULL;
         stopsOnly = stopsOnly->next);
    if (stopsOnly != NULL && stopsOnly->stop != NULL) {
        lastName = malloc(strlen(stopsOnly->stop) + 1);
        strcpy(lastName, stopsOnly->stop);
    }

    if (route->headStopsOnly == NULL || route->headStopsOnly->stop == NULL) {
        addStopToRoute(route, fromName, cost, duration);
        insertStopAfter(route, toName, cost, duration);
    }
    else if (strcmp(lastName, fromName) == 0)
        insertStopAfter(route, toName, cost, duration);
    else if (strcmp(firstName, toName) == 0)
        insertStopBefore(route, fromName, cost, duration);
    else {
        puts("link cannot be associated with bus line.");
        return;
    }
    
    for (routesOnly = stopFrom->headRoutesOnly; routesOnly != NULL && routesOnly->route != NULL &&
         strcmp(routesOnly->route, routeName) != 0; routesOnly = routesOnly->next);
    if (routesOnly == NULL || routesOnly->route == NULL)
        addRouteToStop(stopFrom, routeName);
    for (routesOnly = stopTo->headRoutesOnly; routesOnly != NULL && routesOnly->route != NULL &&
         strcmp(routesOnly->route, routeName) != 0; routesOnly = routesOnly->next);
    if (routesOnly == NULL || routesOnly->route == NULL)
        addRouteToStop(stopTo, routeName);
    
    route->costTotal += cost;
    route->durationTotal += duration;
    
}


/* Lists the interseptions */
void iCommandNoArgs(linkStop headStop) {
    int counter;
    linkStop stop;
    linkRoutesOnly routes;
    
    for (stop = headStop; stop != NULL; stop = stop->next) {
        routes = stop->headRoutesOnly;
        bubbleSort(routes);
        for (counter = 0; routes != NULL; counter++, routes = routes->next);
        if (counter > 1) {
            printf("%s %d:", stop->name, counter);
            for (routes = stop->headRoutesOnly; routes != NULL; routes = routes->next) {
                printf(" %s", routes->route);
            }
            putchar('\n');
        }
    }
}

/* Removes a route */
void rCommandOneArgs(linkRoute *headRoute, linkStop *headStop, char *name) {
    linkRoute route, newRoute;
    linkStopsOnly stopsOnly, stopsOnlyNew;
    linkStop stop;
    linkRoutesOnly routesOnlyOld, routesOnly, routesOnlyNew;
    
    for (route = *headRoute, newRoute = NULL;
         route != NULL && strcmp(route->name, name) != 0;
         newRoute = route, route = route->next);

    if (route == NULL) {
        printf("%s: no such line.\n", name);
        return;
    }

    for (stopsOnly = route->headStopsOnly; stopsOnly != NULL; ) {
        stopsOnlyNew = stopsOnly->next;
        stopsOnly = stopsOnlyNew;
    }

    if (route == *headRoute)
        *headRoute = route->next;
    else
        newRoute->next = route->next;

    free(route->name);
    free(route);

    for (stop = *headStop; stop != NULL; stop = stop->next) {
        for (routesOnly = stop->headRoutesOnly, routesOnlyNew = NULL; routesOnly != NULL; ) {
            if (routesOnly->route != NULL && strcmp(routesOnly->route, name) == 0) {
                if (routesOnly == stop->headRoutesOnly) {
                    stop->headRoutesOnly = routesOnly->next;
                    if (routesOnly->next != NULL)
                        routesOnly->next->prev = NULL;
                }
                else {
                    routesOnlyNew->next = routesOnly->next;
                    if (routesOnly->next != NULL)
                        routesOnly->next->prev = routesOnlyNew;
                }

                routesOnlyOld = routesOnly;
                routesOnly = routesOnly->next;
                free(routesOnlyOld->route);
                free(routesOnlyOld);
                continue;
            }
            routesOnlyNew = routesOnly;
            routesOnly = routesOnly->next;
        }
    }
}

/* Removes a stop */
void eCommandOneArgs(linkRoute *headRoute, linkStop *headStop, char *name) {
    linkStop stop, newStop;
    linkRoutesOnly routesOnly, routesOnlyNew;
    linkRoute route;
    linkStopsOnly stopsOnlyOld, stopsOnly, stopsOnlyNew;
    
    for (stop = *headStop, newStop = NULL; stop != NULL && strcmp(stop->name, name) != 0;
         newStop = stop, stop = stop->next);

    if (stop == NULL) {
        printf("%s: no such stop.\n", name);
        return;
    }

    for (routesOnly = stop->headRoutesOnly; routesOnly != NULL; ) {
        routesOnlyNew = routesOnly->next;
        routesOnly = routesOnlyNew;
    }

    if (stop == *headStop)
        *headStop = stop->next;
    else
        newStop->next = stop->next;

    free(stop->name);
    free(stop);
    
    for (route = *headRoute; route != NULL; route = route->next) {
        for (stopsOnly = route->headStopsOnly, stopsOnlyNew = NULL; stopsOnly != NULL; ) {
            if (stopsOnly->stop != NULL && strcmp(stopsOnly->stop, name) == 0) {
                if (route->headStopsOnly == stopsOnly) {
                    route->durationTotal -= stopsOnly->durationLocal;
                    route->costTotal -= stopsOnly->costLocal;
                }
                if (stopsOnly->next == NULL) {
                    route->durationTotal -= stopsOnly->durationLocal;
                    route->costTotal -= stopsOnly->costLocal;
                }
            
                if (stopsOnly == route->headStopsOnly) {
                    route->headStopsOnly = stopsOnly->next;
                    if (stopsOnly->next != NULL)
                        stopsOnly->next->prev = NULL;
                }
                else {
                    stopsOnlyNew->next = stopsOnly->next;
                    if (stopsOnly->next != NULL)
                        stopsOnly->next->prev = stopsOnlyNew;
                }
                
                stopsOnlyOld = stopsOnly;
                stopsOnly = stopsOnly->next;
                free(stopsOnlyOld->stop);
                free(stopsOnlyOld);
                continue;
            }
            stopsOnlyNew = stopsOnly;
            stopsOnly = stopsOnly->next;
        }
        if (route->headStopsOnly == NULL) {
            route->durationTotal = 0.00;
            route->costTotal = 0.00;
        }
        else
            if (route->headStopsOnly->next != NULL && route->headStopsOnly->next->stop != NULL)
                if (strcmp(route->headStopsOnly->stop, route->headStopsOnly->next->stop) == 0) {
                    route->durationTotal -= route->headStopsOnly->durationLocal;
                    route->costTotal -= route->headStopsOnly->costLocal;
                }
                
    }
}

/* Free everything */
void aCommandNoArgs(linkRoute *headRoute, linkStop *headStop) {
    linkRoute route, nextRoute;
    linkStopsOnly stopOnly, nextStopOnly;
    linkStop stop, nextStop;
    linkRoutesOnly routeOnly, nextRouteOnly;

    for (route = *headRoute; route != NULL; route = nextRoute) {
        nextRoute = route->next;
        for (stopOnly = route->stopsAll; stopOnly != NULL; stopOnly = nextStopOnly) {
            nextStopOnly = stopOnly->next;
            if (stopOnly->stop != NULL) {
                free(stopOnly->stop);
            }
            free(stopOnly);
        }
        free(route->name);
        free(route);
    }
   
    for (stop = *headStop; stop != NULL; stop = nextStop) {
        nextStop = stop->next;
        for (routeOnly = stop->routesAll; routeOnly != NULL; routeOnly = nextRouteOnly) {
            nextRouteOnly = routeOnly->next;
            if (routeOnly->route)
                free(routeOnly->route);
            free(routeOnly);
        }
        free(stop->name);
        free(stop);
    }
}

/* Main function that runs is responsible to run the commands (q, c, p, l, i, e, r, a) */
int main(void) {
    char *lineString, **lineArray;
    linkRoute headRoute = NULL;
    linkStop headStop = NULL;
    size_t len = 0;
    ssize_t slen;
    int i;
    
    for (lineString = NULL, slen = getline(&lineString, &len, stdin); slen != EOF;
         lineString = NULL, slen = getline(&lineString, &len, stdin)) {
        if (strlen(lineString) > 1) {
            lineString[slen-1] = '\0';
            lineArray = stringToArray(lineString);
            if (strcmp(lineArray[0], "q") == 0) {
                free(lineArray);
                break;
            }
            else if (strcmp(lineArray[0], "c") == 0 && size(lineArray) == 1)
                cCommandNoArgs(headRoute);
            else if (strcmp(lineArray[0], "c") == 0 && size(lineArray) == 2)
                headRoute = cCommandOneArgs(headRoute, lineArray[1]);
            else if (strcmp(lineArray[0], "c") == 0 && size(lineArray) == 3)
                cCommandTwoArgs(headRoute, lineArray[1], lineArray[2]);
            else if (strcmp(lineArray[0], "p") == 0 && size(lineArray) == 1)
                pCommandNoArgs(headStop);
            else if (strcmp(lineArray[0], "p") == 0 && size(lineArray) == 2)
                pCommandOneArgs(headStop, lineArray[1]);
            else if (strcmp(lineArray[0], "p") == 0 && size(lineArray) == 4)
                headStop = pCommandThreeArgs(headStop, lineArray[1], atof(lineArray[2]),
                                             atof(lineArray[3]));
            else if (strcmp(lineArray[0], "l") == 0 && size(lineArray) == 6)
                lCommandFiveArgs(&headStop, &headRoute, lineArray[1], lineArray[2], lineArray[3],
                                 atof(lineArray[4]), atof(lineArray[5]));
            else if (strcmp(lineArray[0], "i") == 0 && size(lineArray) == 1)
                iCommandNoArgs(headStop);
            else if (strcmp(lineArray[0], "r") == 0 && size(lineArray) == 2)
                rCommandOneArgs(&headRoute, &headStop, lineArray[1]);
            else if (strcmp(lineArray[0], "e") == 0 && size(lineArray) == 2)
                eCommandOneArgs(&headRoute, &headStop, lineArray[1]);
            else if (strcmp(lineArray[0], "a") == 0 && size(lineArray) == 1) {
                aCommandNoArgs(&headRoute, &headStop);
                headRoute = NULL;
                headStop = NULL;
            }
            for (i = 0; lineArray[i]; i++)
                free(lineArray[i]);
            free(lineArray);
        }
    }
    free(lineString);
    return 0;
}
