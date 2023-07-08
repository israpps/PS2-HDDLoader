#ifdef FREEMCBOOT_ALA
void CheckHDDUpdate(int device, const char *SysExecFolder);
#else
extern unsigned char IsHddSupportEnabled;
void construct_HDDLOAD_argvc(char* CmdStr);
#endif
int GetHddSupportEnabledStatus(void);
int GetHddUpdateStatus(void);
void DetermineHDDLoadStatus(void);
