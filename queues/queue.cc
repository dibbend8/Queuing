/*
 * Steps:
 * 1. Store the arrivals in a queue
 * 2. If buffer is empty (implies server is free), send to server
 * 3. If buffer is not empty, insert arrival in queue and wait for server to be free
 * 4. When server instructs, delete the HOL and send new request to server if buffer is non empty
 */

#include "queue.h"

Define_Module(Queue);
Queue::~Queue()
{
    buffer.clear();
}
void Queue::initialize()
{
    // TODO - Generated method body
    buffcap = par("capacity").intValue();
    customer  = cHistogram("customer");
}

void Queue::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    if(strcmp(msg->getName(),"newarr")==0 && buffer.getLength()<buffcap)
    {
        customer.collect(buffer.getLength());// statistics collection
        //printing buffer status
        EV<<"Buffer status = "<<buffer.isEmpty()<<" Buffer Length = "<<buffer.getLength()<<endl;
        //Check if buffer is empty (implies server is free on arrival)
        if (buffer.isEmpty())
        {
            //send to server for serving
            cMessage *serve = new cMessage("serve");
            send(serve,"out");
        }
        buffer.insert(new cMessage("arr"));
        delete msg;
    }
    else if(strcmp(msg->getName(),"clear")==0)
    {
        //server instructs service is over. delete the HOL from buffer
        cMessage *del = (cMessage *)buffer.pop();
        delete del;
        //if buffer is non-empty, send the new HOL to the server
        if(!buffer.isEmpty())
        {
            cMessage *serve = new cMessage("serve");
            send(serve,"out");
        }
        delete msg;
    }

}
void Queue::finish()
{
    cout<<"Mean = "<<customer.getMean()<<endl;
    cout<<"Stdev = "<<customer.getVariance()<<endl;

}
