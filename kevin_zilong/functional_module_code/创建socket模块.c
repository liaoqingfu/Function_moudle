int zx_mgw_commun_create_socket(char *serv_ip,unsigned int serv_port)
{
	struct sockaddr_in servaddr;
	int sockfd = -1;
	int ret = 0; 
	
	if(!serv_ip )
	{
		dzlog_error("The function parameter  is NULL");
		return -1;
	}

	dzlog_info("mgw commun create socket start");
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd < 0)
	{
		dzlog_error("*** socket error : %d", sockfd);
		return -1;
	}
	
	dzlog_info("create socket is ok");
	
	bzero(&servaddr, sizeof(servaddr));
	
	servaddr.sin_family = AF_INET;
	
	inet_pton(AF_INET, serv_ip, &servaddr.sin_addr.s_addr);
	
	servaddr.sin_port = htons(serv_port);

	ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	if(ret < 0)
	{
		dzlog_error("*** connect error : %d", ret);
		return -1;
	}
	dzlog_info(" connect is ok ");

	return sockfd;
	
}
