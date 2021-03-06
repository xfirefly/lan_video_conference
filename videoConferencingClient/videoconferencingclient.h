#ifndef VIDEOCONFERENCINGSERVER_H
#define VIDEOCONFERENCINGSERVER_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <QObject>
#include <string>
#include <QJsonObject>
#include "employee.h"

#define BUFFER_LENGTH_TCP     5000
#define BUFFER_LENGTH_UDP     1048

using namespace boost::asio;
using boost::asio::ip::tcp;
using boost::system::error_code;
using boost::asio::ip::address;


class VideoConferencingClient
{
public:
    VideoConferencingClient();
    //TCP（调用HANDLE部分）
    void threadTcpReceive();//TCP-接收 线程
    void tcpReceiveMessage();//TCP-接收数据
    void tcpStrResultAnalysis(std::string str);//TCP-对 接收的数据 的处理

    //TCP--处理在线转发部分
    void accept(std::string ip);
    void threadTcpOnlineReceive();
    void tcpOnlineReceiveMessage();
    //UDP--处理在线转发部分
    void threadUdpOnlineReceive();
    void udpOnlineReceiveMessage();
    //在线转发数据分析
    void onlineStrResultAnalysis(std::string str);



    //RTP
    void threadRtpReceive();//RTP-接收 线程
    void rtpReceiveMessage();//RTP-接收数据
    void rtpResultAnalysis();//RTP-对 接收的数据 的处理


    //REQUEST PART：客户端 发送 数据 (调用COLLECTION部分)
    void requestRegister(std::string realName, std::string passwd, std::string email, QByteArray avatar, std::string company, std::string department, std::string group, std::string phoneNumber);
    void requestLogin(std::string emailId, std::string passwd);
    void requestExit(std::string emailId);
    void requestAccountDetail(std::string emailId);//请求账户细节
    void requestColleagueList(std::string emaiId);//请求同事列表
    void requestMeetingInvitionsList(std::string emaiId);//请求会议被邀请列表
    void requestMeetingList(std::string emailID);//会议列表
    void requestUnnotedMeetings(std::string userID);//申请未记载的会议
    void requestLaunchMeeting(std::string emailid, std::string assistant, std::string speaker, std::string date, std::string time, std::string category, std::string subject, std::string scale, std::string dura, std::string remark, std::vector<std::string> attendees);
    void requestReplyMeetingInvitation(std::string emailid, std::string result, std::string meetingID, std::string cause);//请求回复会议通知
    void requestStartMeeting(std::string emailid, std::string meetingID);
    void requestStopMeeting(std::string emailid, std::string meetingID);
    void requestSpeak(std::string userID,std::string meetingID);
    void requestAttendMeeting(std::string userID,std::string meetingID);
    void requestStartVideo(std::string userID,std::string meetingID);
    void setEmployee(Employee *employee);
//    void setCompany(Company *company);

private:
    //HANDLE PART：客户端处理来自Server的数据
    void handleRegisteredResult(QJsonObject qo, int &result, QString &returnID, QString &email, QString &err);//处理注册返回结果
    void handleLoginResult(QJsonObject qo, int &result, QString &err);//处理登录返回结果
    void handleInitAccountDetailResult(QJsonObject qo, QList<QString> &employeeDetail);
    void handleInitColleagueListResult(QJsonObject qo);
    void handleInitMeetingInvitionsListResult(QJsonObject qo);
    void handleInitMeetingListResult(QJsonObject qo);
    void handleReplyLaunchMeetingResult(QJsonObject qo, QString &err);
    void handleReplyInvitation(QJsonObject qo);
    //处理在线转发
    void handleOnlineMeetingInvitationResult(QJsonObject qo, QString &speaker, QString &assistant);//处理发起会议的在线转发
    void handleOnlineMeetingResult(QJsonObject qo);//针对speaker在线，assistant发起会议，speaker端会议+1
    void handleOnlineMeetingStartResult(QJsonObject qo);//处理开始会议的在线转发
    void handleOnlineMeetingStopResult(QJsonObject qo);//处理结束会议的在线转发
    void handleOnlineMeetingAttendeesResult(QJsonObject qo);//处理加入会议的在线转发
    void handleOnlineMeetingAttendeeResult(QJsonObject qo);
    void handleOnlineMeetingExitResult(QJsonObject qo);

    //COLLECTION PART：客户端 收集整理 数据
    std::string registerJsonToString(std::string realName, std::string passwd, std::string email, QByteArray avatar, std::string company, std::string department, std::string group, std::string phoneNumber);
    std::string loginJsonToString(std::string emailId, std::string passwd);
    std::string exitJsonToString(std::string emailId);
    std::string initializeAccountDetailJsonToString(std::string emailId);
    std::string initializeColleagueListToString(std::string emailId);
    std::string initializeMeetingInvitionsListToString(std::string emailid);
    std::string initializeMeetingsListToString(std::string emailID);
    std::string initializeUnnotedMeetingsListToString(std::string userID);

    std::string requestLaunchMeetingToString(std::string emailid, std::string assistant, std::string speaker, std::string date, std::string time, std::string category, std::string subject, std::string scale, std::string dura, std::string remark, std::vector<std::string> attendees);
    std::string requestReplyMeetingToString(std::string emailid, std::string result, std::string meetingID, std::string cause);
    std::string requestStartMeetingToString(std::string emailid, std::string meetingID);
    std::string requestStopMeetingToString(std::string emailid, std::string meetingID);
    std::string requestAttendMeetingToString(std::string emailid,std::string meetingID);
    std::string requestStartVideoToString(std::string userID,std::string meetingID);

    void tcpSendMessage(std::string msg);
    QJsonObject stringToQJsonObject(std::string str);

    io_service m_io;
    tcp::socket m_sockTcp;
    tcp::endpoint m_tcpEP;

    boost::array<char,BUFFER_LENGTH_TCP> m_tcpRecvBuf;//接收数据缓冲区。
    boost::array<char,BUFFER_LENGTH_TCP> m_tcpSendBuf;//接收数据缓冲区。

    boost::array<char,BUFFER_LENGTH_UDP> m_udpRecvBuf;//接收数据缓冲区。

    //    ip::tcp::acceptor m_acceptor;

    Employee *m_employee;
    Company com;
    QList<Notification *> noti;
    QList<Meeting *> mee;
    QList<Attendee *> att;
//    Company *m_company;
};

#endif // VIDEOCONFERENCINGSERVER_H
