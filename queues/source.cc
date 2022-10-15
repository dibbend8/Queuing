/*
 * Steps:
 * 1. Get the arrival rate (lam) from the network module parameter
 * 2. Schedule arrivals at exponential interarrivals
 * 3. Send to queue
 */


#include "source.h"

Define_Module(Source);
Source::~Source()
{
    cancelAndDelete(arrival);
}
void Source::initialize()
{
    // TODO - Generated method body


    lam = par("sourcerate").doubleValue();
    arrival = new cMessage("arrival");
    scheduleAfter(exponential(1/lam),arrival);// 1st arrival scheduled after an exponential time

}

void Source::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(msg==arrival)
    {
        cMessage *arr = new cMessage("newarr");
        send(arr,"out"); //sending arrival to queue
        scheduleAfter(exponential(1/lam),arrival);//further arrivals scheduled
    }

}
