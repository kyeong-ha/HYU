import java.io.*;
import java.net.*;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

public class Assignment1_2018008304_KyeongHaPark {

    static MulticastSocket receiveSocket;
    static MulticastSocket senderSocket;
    static MultiCastClient sender;
    static MulitiCastServer receiver;

    static int port;

    public Assignment1_2018008304_KyeongHaPark() {
        try {
            //커맨드문인지 확인하고, 커맨드가 아니라면 계속 인자를 받는다.
            String[] userInput = IsJoinCommand();

            //채팅방 이름을 SHA-256해시를 이용하여 addr로 변환한다.
            String chatName = userInput[1];
            int[] multicastAddrInt = {225, 0, 0, 0};
            System.arraycopy(IPtoSHA256(userInput[1]), 0, multicastAddrInt, 1, 3);
            String multicastAddrStr = Arrays.toString(multicastAddrInt);
            multicastAddrStr = multicastAddrStr.substring(1,multicastAddrStr.length()-1).replace(", ",".");
            
            //peer의 정보를 저장한다.
            String userName = userInput[2];
            
            //소켓이 종료되었는지 스레드로 확인하기 위해 전역변수로 선언해둔다.
            senderSocket = new MulticastSocket();
            receiveSocket = new MulticastSocket(port);

            //수신 스레드 생성를 생성한다.
            receiver = new MulitiCastServer(port, multicastAddrStr, receiveSocket);
            receiver.start();
            
            //송신 스레드 생성
            sender = new MultiCastClient (userName, multicastAddrStr, chatName, port, senderSocket, receiveSocket);
            sender.start();

            //소켓이 종료되었는지 확인하는 스레드를 생성한다. 소켓이 종료되었다는 것은 채팅방에서 나갔다는 의미이므로 새로운 채팅방을 입력받기 위함이다.
            IsSocketClosed isClosed = new IsSocketClosed();
            isClosed.start();
            
        }catch(Exception e){
            System.out.println( "[error] " + e.getMessage());
            System.exit(0);
       }
    }

    public static String[] IsJoinCommand() throws IOException{ 
        //Join 커맨드인지 확인하는 함수 
        //join 커맨드가 아닌 경우 무한반복하여 input을 받음
        System.out.println("-------------------------------------------------------");
        System.out.println("오픈 채팅 프로그램에 오신 것을 환영합니다!\n입장하실 채팅방과 닉네임을 입력해주세요.");
        System.out.println("[ #JOIN (채팅방 이름) (닉네임) ]");
        System.out.println("-------------------------------------------------------");
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in)); 
        while(true){
            String[] str = br.readLine().split("\\s");
            //JOIN 커맨드인 경우 입력값을 저장한다.
            if(str[0].equals("#JOIN") && str.length == 3)
                return str;
            
            //만약 EXIT 커맨드인 경우 프로그램을 종료한다.
            if(str[0].equals("#EXIT") && str.length == 1)
                System.exit(0);
        }
    }

    public static int[] IPtoSHA256(String msg){
        //채팅방 이름을 SHA-256 해시값을 이용하여 멀티캐스트 주소값으로 변환한다.
        int xyz[] = new int[3];
        MessageDigest md;
        try {
            md = MessageDigest.getInstance("SHA-256");
            md.update(msg.getBytes());
            byte[] tempMd = md.digest();
            xyz[2]=Byte.toUnsignedInt(tempMd[tempMd.length-1]);
            xyz[1]=Byte.toUnsignedInt(tempMd[tempMd.length-2]);
            xyz[0]=Byte.toUnsignedInt(tempMd[tempMd.length-3]);
            
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
       return xyz;
    }


    public static void main(String[] args) {
        //실행인자가 format에 맞는지 확인한다. 
        if(args.length != 1 || !args[0].chars().allMatch(Character::isDigit)){
            System.out.println("Port number를 입력해주세요!");
            System.out.println("Example Format: Assingment1_2018008304_KyeongHaPark 8888");
            System.exit(0);
        }

        //port는 실행인자에서 받아야 하므로 main함수에서 저장해준다.
        port = Integer.parseInt(args[0]);

        //멀티캐스트를 이용할 peer를 생성한다.
        Assignment1_2018008304_KyeongHaPark peer = new Assignment1_2018008304_KyeongHaPark();
     }

    static class IsSocketClosed extends Thread{
    //채팅방에서 나간 것을 확인하면 스레드를 종료한 후 다시 채팅방에 입장할 수 있도록 한다.
    public void run() {
        try{
            while(true){
                if(senderSocket.isClosed()){ 
                    System.out.println("-------------------------------------------------------");
                    System.out.println("채팅방과의 연결을 종료하고 있습니다....");
                    sender.sleep(1000);
                    sender.interrupt();
                    receiver.sleep(1000);
                    receiver.interrupt();
                    System.out.println("채팅방과의 연결이 성공적으로 종료되었습니다!");
                    System.out.println("-------------------------------------------------------");

                    //멀티캐스트를 이용할 peer를 재생성한다.
                    Assignment1_2018008304_KyeongHaPark peer = new Assignment1_2018008304_KyeongHaPark();
                    break;
                }
            }
            
        }catch(Exception e){
                System.out.println( "[error] " + e.getMessage());
                System.exit(0);
        }
    }
    
    }
}



class MulitiCastServer extends Thread{
    int port;
    String multicastAddrStr;
    MulticastSocket receiveSocket;


    MulitiCastServer(int p, String addr, MulticastSocket rs){
        port = p;
        multicastAddrStr = addr;
        receiveSocket = rs;
    }
      public void run(){
            try{
           receiveSocket.joinGroup(InetAddress. getByName(multicastAddrStr));
           
                      while( true){
                        byte []msg = new byte[512];
                           DatagramPacket packet = new DatagramPacket(msg, msg.length );
                          
                           receiveSocket.receive(packet);
                           
                           String str = new String(packet.getData()); 
                           
                           System. out.println(str);
                     }
           }
            catch(Exception e){
           }
     }

}



class MultiCastClient  extends Thread {
    String userName;
    String multicastAddrStr;
    String chatName;
    int port;
    MulticastSocket senderSocket;
    MulticastSocket receiveSocket;

    MultiCastClient(String name, String addr,String chat, int p, MulticastSocket ss, MulticastSocket rs){
        userName = name;
        multicastAddrStr = addr;
        port = p;
        chatName = chat;
        senderSocket = ss;
        receiveSocket = rs;
    }

      public void run(){
            try{

           InetAddress multicastAddress = InetAddress.getByName(multicastAddrStr);

           String accessMsg = "-------------------------------------------------------\n"
        + "어서오세요! <" + chatName + "> 채팅방 입니다.\n" + userName + "님이 입장하셨습니다.\n"
        + "-------------------------------------------------------";
           DatagramPacket accessPacket = new DatagramPacket(accessMsg.getBytes(), accessMsg.getBytes().length , multicastAddress, port );
                senderSocket.send(accessPacket);

           BufferedReader br = new BufferedReader( new InputStreamReader(System.in ));
           
                 while(true){

                     String msg = br.readLine(); 

                    //EXIT 커맨드인 경우 채팅방을 나간다.
                    if(msg.equals("#EXIT")){
                            
                            String logoutMsg = "====================================================\n"
                            + userName + "님이 로그아웃 하셨습니다.\n" +
                            "====================================================";
                            DatagramPacket logoutPacket = new DatagramPacket(logoutMsg.getBytes(), logoutMsg.getBytes().length , multicastAddress, port );
                            senderSocket.send(logoutPacket);

                            
                            senderSocket.close();
                            receiveSocket.close();
                            receiveSocket.leaveGroup(multicastAddress);
                            senderSocket.leaveGroup(multicastAddress);
                
                    }
                    else if(msg.charAt(0)=='#'){}
                    else{
                        msg = userName + ": " + msg;
                        DatagramPacket packet = new DatagramPacket(msg.getBytes(), msg.getBytes().length , multicastAddress, port );
                     
                        senderSocket.send(packet);
                    }
                               
                }
           
                
                
           }
            catch(Exception e){}
                
                
                
                
     }
     
}

