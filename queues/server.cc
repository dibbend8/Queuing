/*
 * Steps:
 * 1. On receiving a service request, schedule an end service event after an exponentially distributed interval
 * 2. When service over, tell queue to delete the HOL.
 */

#include "server.h"

Define_Module(Server);
Server::~Server()
{
    cancelAndDelete(servend);
}
void Server::initialize()
{
    // TODO - Generated method body
    isBusy = false;
    mu = par("servrate").doubleValue();
    servend = new cMessage("endservice");
}

void Server::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(strcmp(msg->getName(),"serve")==0)
    {
        isBusy = true;
        scheduleAfter(exponential(1/mu),servend);
        delete msg;
    }
    if(msg==servend)
    {
        isBusy=false;
        cMessage *clear = new cMessage("clear");
        send(clear,"out");
    }
}
