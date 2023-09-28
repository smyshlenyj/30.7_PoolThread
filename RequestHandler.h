#include "ThreadPool.h" 

class RequestHandler {
   public:
       RequestHandler();
       ~RequestHandler();
       void pushRequest(FuncType, std::shared_ptr<int[]>, long, long);

   private:
       ThreadPool m_tpool;
};