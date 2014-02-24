
onstart_init:
	//加载模块：
		xt_mac
		xt_mark
		ipt_webstr
		ipt_ipp2p
		nf_nat_pptp
		nf_conntrack_pptp
		nf_nat_proto_gre

	//设置参数
		nvram set no_crossdetect=1
		nvram commit



onfactory_init:

	//更新环境
	source /usr/sbin/basic_env

	//在线注册设备
	--已经注册：online_setup = 1
	--新设备：
		---ping 通服务器
		---服务器取回设备编号
		---设置设备编号nvram set router_name=ZMDevice_0XX
				 nvram set lan_ipaddr=192.168.2.1
		---更新online_setup = 1


	//设备变量初始化
	--注册失败：60S重启
	--注册成功
	---是否初始化 nvram get onfactory_init_v99
	---没有初始化 
		----COMMON_ONFACTORY_INIT
		----SPECIAL_ONFACTORY_INIT



z_monitor:
	 //更新环境
	 source /usr/sbin/basic_env
	 //初始化服务
	 mkdir /tmp/auth_server
	 thttpd -p 8901 -d /tmp/ -u root -c **.cgi
	 touch /tmp/keepalive.js
	 echo "httpon" > /tmp/keepalive.js
	"httpd_monitor &"------------------------>
	

	//检测zmeng状态：300S循环
	--检测PING：阻塞
	 	serv_ping_response = pong
	--PING不通：结束WIFIDOG 和ZMENG
		awctl stop;
		kill -9 `cat /tmp/zmeng.pid`

	--PING 通：
		---查看ZMENG是否存在
		ps | grep zmeng | awk '{print $5}' | grep $if_zmeng
		----不存在：重启/usr/sbin/zmeng
		----存在：zmeng_state=“OK”

		---重启AUTODOG：重复
		---启动cli_z 进行服务器命令更新


httpd_monitor:
	//保证http存活
		--httpd_alive="httpon"?
		---不在：
		kill -9 `ps | grep httpd | grep thttpd | awk '{print $1}'`
		thttpd -p 8901 -d /tmp/ -u root -c **.cgi
		touch /tmp/keepalive.js
		echo "httpon" > /tmp/keepalive.js
		sleep 5



z_upgrade:
	//查看空间情况
	cat /proc/meminfo | grep MemFree | awk '{print $2}
	//检查版本号码
	 cur_version=`cat /usr/sbin/cur_version`
	//升级
	filename=`basename $Ip`
	mkfifo /tmp/$filename
	wget $Ip -O /tmp/$filename &
	write /tmp/$filename linux
	reboot






