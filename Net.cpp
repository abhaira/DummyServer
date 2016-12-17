class NetSelect
{
   public:
      void addReadFd(int fd)
      {
	 FD_SET(fd, &m_readFds);
	 ++m_fdCount;
      }
      void removeReadFd(int fd)
      {
	 FD_CLR(fd, m_readFds);
	 --m_fdCount;
      }
      void addWriteFd(int fd)
      {
	 FD_SET(fd, &m_writeFds);
	 ++m_fdCount;
      }
      void removeWriteFd(int fd)
      {
	 FD_CLR(fd, &m_writeFds);
	 --m_fdCount;
      }

   private:
      fd_set m_readFds;
      fd_set m_writeFds;
      fd_set m_expFds;
      fd_set m_readFdsCpy;
      fd_set m_writeFdsCpy;
      fd_set m_expFdsCpy;
      int m_fdCount;
};
