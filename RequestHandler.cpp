#include "RequestHandler.h"

RequestHandler::RequestHandler() 
{
   m_tpool.start();
}

RequestHandler::~RequestHandler() 
{
   m_tpool.stop();
}

void RequestHandler::pushRequest(FuncType f, std::shared_ptr<int[]> array, long a, long b) 
{
   m_tpool.push_task(f, array, a, b);
}