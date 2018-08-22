# LM项目

LM软件全称为Local Message，用于局域网通信的聊天软件。

## 模块
- lm_ctrl
- lm_chat_daemon
- lm_chat
- lm_file_recv
- lm_file_send

### lm_ctrl
lm_ctrl负责获取用户输入的控制命令的处理、广播信息、文件传输控制的处理。

### lm_chat
lm_chat负责点对点聊天。

### lm_file_recv
lm_file_recv负责文件接收。

### lm_file_send
lm_file_send负责文件发送。

## 流程图
### 启动流程
lm_ctrl启动之后，立即fork+exec执行lm_file_send和lm_file_recv进程。

###

```{mermaid}
sequenceDiagram
lmctrl->>所有人: 发起TCP连接
客户端->>服务器: http请求报文
服务器->>客户端: http响应报文
客户端->>服务器: 关闭连接
```
