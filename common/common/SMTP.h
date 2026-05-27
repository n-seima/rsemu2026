// SMTP.h: interface for the CSMTP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMTP_H__19805707_F1BC_43FA_9117_28B962F4164D__INCLUDED_)
#define AFX_SMTP_H__19805707_F1BC_43FA_9117_28B962F4164D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <windows.h>

const int DEFAULT_PROTOCOL = 0;
const int NO_FLAGS = 0;

class cSmtp  
{
public:        
    cSmtp();    
    virtual ~cSmtp();
    bool    AddRecipient(const char email[], const char name[]="");
    bool    AddBCCRecipient(const char email[], const char name[]="");
    bool    AddCCRecipient(const char email[], const char name[]="");    
    bool    ConnectServer(const char server[], const unsigned short port=NULL);
    bool    Disconnect();
    bool    GetConnectStatus();    
    const unsigned int    GetBCCRecipientCount();    
    const unsigned int    GetCCRecipientCount();
    const unsigned int    GetRecipientCount();    
    const unsigned int    GetSocket();
    const char*    const    GetLocalHostIp();
    const char*    const    GetLocalHostname();    
    const char*    const    GetMessageBody();    
    const char*    const    GetReplyTo();
    const char*    const    GetSenderEmail();
    const char*    const    GetSenderName();
    const char*    const    GetSubject();    
    const char*    const    GetXMailer();    
    bool    Send();
    void    SetMessageBody(const char body[]);    
    void    SetSubject(const char subject[]);    
	void	SetSenderEmail(const char name[], const char email[]);
    void    SetSenderName(const char name[]);    
    void    SetSenderEmail(const char email[]);    
    void    SetReplyTo(const char replyto[]);    
    void    SetXMailer(const char xmailer[]);

private:
    class CRecipient
    {
    public:
        CRecipient() 
        { 
            m_pcEmail = NULL;
        };
        CRecipient& operator=(const CRecipient& src)
        {
            if (&src != this)
            {
                if (m_pcEmail)
                    delete [] m_pcEmail;
                int s = strlen(src.m_pcEmail);
                m_pcEmail = new char[s+1];
                strcpy(m_pcEmail, src.m_pcEmail);
            }
            return (*this);
        };
        virtual ~CRecipient()
        {
            if (m_pcEmail)
                delete [] m_pcEmail;
        };
        char* GetEmail()
        {
            return m_pcEmail;
        };
        void SetEmail(const char email[])
        {
//            assert(email);
            int s=strlen(email);
            if (s > 0)
            {
                m_pcEmail = new char[s+1];
                strcpy(m_pcEmail, email);
            }        
        };
    private:
        char *m_pcEmail;
    };
    bool bCon;
    char m_cHostName[MAX_PATH];
    char* m_pcFromEmail;
    char* m_pcFromName;
    char* m_pcSubject;
    char* m_pcMsgBody;
    char* m_pcXMailer;
    char* m_pcReplyTo;
    char* m_pcIPAddr;    

    WSADATA wsaData;
    SOCKET hSocket;

    std::vector<CRecipient*> Recipients;
    std::vector<CRecipient*> CCRecipients;
    std::vector<CRecipient*> BCCRecipients;    
    
    char*   _formatHeader();    
    bool    _formatMessage();
    SOCKET  _connectServerSocket(const char* server, const unsigned short port=NULL);    
};


#endif // !defined(AFX_SMTP_H__19805707_F1BC_43FA_9117_28B962F4164D__INCLUDED_)
