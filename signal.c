/*** Disposes of Unwashed Zombie Hordes ***/
void reapz()
{
	int pid = 0;
	int status = 0;

	while(1) 
	{
		pid = waitpid(-1, &status, WNOHANG); 
		if (pid <= 0)
			break;
	}
}

