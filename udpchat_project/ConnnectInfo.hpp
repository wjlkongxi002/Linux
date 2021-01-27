#pragma once 
#include <stdio.h>
#include <string.h>
#include <iostream>

//注册请求的数据格式
/*
 * 昵称
 * 学校
 * 用户的密码
 * client to server 严格按照Register格式传输注册数据
 * */

struct RegisterInfo
{
  RegisterInfo()
  {
    memset(nick_name_, '\0', sizeof(nick_name_));
    memset(school_, '\0', sizeof(school_));
    memset(passwd_,'\0', sizeof(passwd_));
  }
  char nick_name_[20];
  char school_[20];
  char passwd_[20];
};

// 登录请求的数据格式
/*
 *用户id
 密码
 * */

struct LoginInfo
{
  LoginInfo()
  {
    memset(passwd_, '\0', sizeof(passwd_));
  }
  uint16_t id_;
  char passwd_[20];
};

// 服务端给客户端恢复应答的数据格式
/*
 *当前的状态（注册的状态， 登录状态）
 返回用户id，类似于注册完毕之后返回的QQ号
 *
 * */


struct ReplyInfo
{
  int resp_status_;
  uint32_t id_;
};

// 枚举状态  为resp_status_ 准备的

enum status
{
  REGISTER_FAILED = 0,    // 注册失败
  REGISTER_SUCCESS,       // 注册成功
  LOGIN_FAILED,           // 登录失败
  LOGIN_SUCCESS           // 登录成功
};

// 如何标识当前的请求是注册请求还是登录请求

/*
 *对于每一种请求，在tcp层面都会发送两个tcp包
 *第一个tcp包，发送一个字节，标识请求是 注册 还是 登录
 *第二个tcp包，发送具体的 注册 或者 登录 的请求数据
 *
 * */
enum ResqType
{
  REGISTER_RESQ = 0,   // 注册请求标识
  LOGIN_RESQ           // 登录请求标识

};


