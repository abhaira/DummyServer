#include<iostream>
#include<sys/socket.h>


class NetSelect
{
   int maxFdCount()
   {
      return std::max(std::max(m_readFdCount, m_writeFdCount), m_expFdCount);
   }
   void resetFds()
   {
	 FD_ZERO(&m_readFds);
	 FD_ZERO(&m_writeFds);
	 FD_ZERO(&m_expFds);
   }
   void resetCpyFds()
   {
	 FD_ZERO(&m_readFdsCpy);
	 FD_ZERO(&m_writeFdsCpy);
	 FD_ZERO(&m_expFdsCpy);
   }
   public:

      NetSelect()
      {
	 resetFds();
	 resetCpyFds();
	 m_timeout.tv_sec = 0;
	 m_timeout.tv_usec = 0;
	 m_readFdCount = 1;
	 m_writeFdCount =1;
	 m_expFdCount = 1;
      }

      void addReadFd(int fd)
      {
	 FD_SET(fd, &m_readFds);
	 ++m_readFdCount;
      }
      void removeReadFd(int fd)
      {
	 FD_CLR(fd,&m_readFds);
	 --m_readFdCount;
      }
      void addWriteFd(int fd)
      {
	 FD_SET(fd, &m_writeFds);
	 ++m_writeFdCount;
      }
      void removeWriteFd(int fd)
      {
	 FD_CLR(fd, &m_writeFds);
	 --m_writeFdCount;
      }

      void setTimeout(long time)
      {
	 m_timeout.tv_sec = time;
	 m_timeout.tv_usec = 0;
      }

      enum class NetStatus
      {
	 FAILED,
	 SUCCESS,
	 NOACTION
      };


      NetStatus Select()
      {
	 m_readFdsCpy = m_readFds;
	 m_writeFdsCpy = m_writeFds;
	 m_expFdsCpy = m_expFds;

	 NetStatus retStatus = NetStatus::FAILED;

	 int retCode = select(maxFdCount(), &m_readFdsCpy, &m_writeFdsCpy, &m_expFdsCpy, &m_timeout);
	 
	 switch(retCode)
	 {
	    case -1:
	       retStatus = NetStatus::FAILED;
	       resetCpyFds();
	       break;
	    case 0:
	       retStatus = NetStatus::NOACTION;
	       break;
	    default:
	       retStatus = NetStatus::SUCCESS;
	       break;
	 }
	 return retStatus;
      }

      struct NetContext
      {
	 NetContext(fd_set _rd, fd_set _wr, fd_set _exp):
	    m_readFds(_rd),
	    m_writeFds(_wr),
	    m_expFds(_exp)
	 {}

	 bool isFdReadyForRead(int fd)
	 {
	    return 1 == FD_ISSET(fd, &m_readFds);
	 }
	 bool isFdReadForWrite(int fd)
	 {
	    return 1 == FD_ISSET(fd, &m_writeFds);
	 }

	 fd_set m_readFds;
	 fd_set m_writeFds;
	 fd_set m_expFds;
      };
	 
   private:
      fd_set m_readFds;
      fd_set m_writeFds;
      fd_set m_expFds;
      fd_set m_readFdsCpy;
      fd_set m_writeFdsCpy;
      fd_set m_expFdsCpy;
      timeval m_timeout;
      int m_readFdCount;
      int m_writeFdCount;
      int m_expFdCount;
};
