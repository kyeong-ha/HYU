// SPDX-License-Identifier: GPL-3.0
pragma solidity >=0.7.0 <0.9.0;

import "./IRoomShare.sol";

contract RoomShare is IRoomShare{
  uint256 private roomId; //총 등록된 방의 개수
  uint256 private rentId; //총 렌트된 방의 개수

  constructor(){
     roomId = 0;
     rentId = 0;
  }

  mapping (address => Rent[]) private renter2rent;
  mapping (uint => Rent[]) private roomId2rent;
  mapping (uint => Room) private roomId2room;

  function getRoomId() public view override returns(uint256){
    return roomId;
  }
  function getroomId2room(uint _roomId) public view override returns(Room memory){
    return roomId2room[_roomId];
  }

  function getMyRents() public view override returns(Rent[] memory) {
    /* 함수를 호출한 유저의 대여 목록을 가져온다. */
    return renter2rent[msg.sender];
  }

  function getRoomRentHistory(uint _roomId) public view override returns(Rent[] memory) {
    /* 특정 방의 대여 히스토리를 보여준다. */
    return roomId2rent[_roomId];
  }
  
  function shareRoom(string calldata name, string calldata location, uint price) public override {
    /* 1. isActive 초기값은 true로 활성화, 함수를 호출한 유저가 방의 소유자이며, 
          365 크기의 boolean 배열을 생성하여 방 객체를 만든다. */
    bool[] memory isRented = new bool[](365);
    for(uint i = 0; i < 365; i++)
      isRented[i] = false;

    Room memory targetRoom = Room(roomId, name, location, true, price, msg.sender, isRented);

    /* 2. 방의 id와 방 객체를 매핑한다. */
    roomId2room[roomId] = targetRoom;
    //roomId++;
    emit NewRoom(roomId++);
  }

  function rentRoom(uint _roomId, uint checkInDate, uint checkOutDate) payable external override{
    /**
     * 1. roomId에 해당하는 방을 조회하여 아래와 같은 조건을 만족하는지 체크한다.
     *    a. 현재 활성화(isActive) 되어 있는지
     *    b. 체크인날짜와 체크아웃날짜 사이에 예약된 날이 있는지 
     *    c. 함수를 호출한 유저가 보낸 이더리움 값이 대여한 날에 맞게 지불되었는지(단위는 1 Finney, 10^15 Wei) 
     * 2. 방의 소유자에게 값을 지불하고 (msg.value 사용) createRent를 호출한다.
     * *** 체크아웃 날짜에는 퇴실하여야하며, 해당일까지 숙박을 이용하려면 체크아웃날짜는 그 다음날로 변경하여야한다. ***
     */
    Room memory targetRoom = roomId2room[_roomId];

    uint8 decimals = 15;
    uint dayPrice = targetRoom.price * (10 ** uint(decimals));

    //a. 현재 활성화 상태가 아니면, 에러메세지 발생
    require(targetRoom.isActive, "NotActive");

    //b. 해당 날짜가 렌트되어 있는 상태면, 렌트 불가능한 상태로 변경
    for(uint i = checkInDate; i < checkOutDate; i++){
      if(targetRoom.isRented[i-1] == true){  
        require(false, "NotCheckDate");
      }
    }

    //c. 이더리움 값 지불이 올바르지 않으면, 렌트 불가능한 상태로 변경
    uint totalPrice = dayPrice * (checkOutDate - checkInDate);
    require((totalPrice == msg.value), "NotMatchPrice");

    //2. 조건 만족시, 방의 소유자에게 값을 지불하고 createRent 호출
    _sendFunds(targetRoom.owner, msg.value);
    _createRent(_roomId, checkInDate, checkOutDate);
  }

  function _createRent(uint256 _roomId, uint256 checkInDate, uint256 checkOutDate) public override{
    /* 1. 함수를 호출한 사용자 계정으로 대여 객체를 만들고, 
          변수 저장 공간에 유의하며 체크인날짜부터 체크아웃날짜에 해당하는 배열 인덱스를 체크한다.
          (초기값은 false이다.). */
    Rent memory targetRent = Rent(rentId, _roomId, checkInDate, checkOutDate, msg.sender);
    for(uint i = checkInDate; i < checkOutDate; i++){
     roomId2room[_roomId].isRented[i-1] = true;
    }
    /* 2. 계정과 대여 객체들을 매핑한다. (대여 목록) */
    renter2rent[msg.sender].push(targetRent);
    /* 3. 방 id와 대여 객체들을 매핑한다. (대여 히스토리) */
    roomId2rent[_roomId].push(targetRent);

    emit NewRent(_roomId, rentId++);
  }

  function _sendFunds (address owner, uint256 value) public override{
      payable(owner).transfer(value);
  }

  function recommendDate(uint _roomId, uint checkInDate, uint checkOutDate) public view override returns(uint[2] memory) {
    /**
     * 대여가 이미 진행되어 해당 날짜에 대여가 불가능 할 경우, 
     * 기존에 예약된 날짜가 언제부터 언제까지인지 반환한다.
     * checkInDate(체크인하려는 날짜) <= 대여된 체크인 날짜 , 대여된 체크아웃 날짜 < checkOutDate(체크아웃하려는 날짜)
     */

    Room memory targetRoom = roomId2room[_roomId];
    uint[2] memory checkDate = [checkInDate, checkOutDate];

    // 해당 날짜가 렌트되어 있는 상태면, 대여정보를 찾는다.
    for(uint i = checkInDate; i < checkOutDate; i++){

      if(targetRoom.isRented[i-1] == true) {
        Rent[] memory targetRent = roomId2rent[_roomId];
        // 대여하고 있는 방 정보 중에서 해당 날짜를 포함하고 있는 방을 찾는다.
        // 해당하는 방의 체크인 날짜와 체크아웃 날짜를 배열에 저장한다.
        for(uint j = 0; j < targetRent.length; j++){
          if(targetRent[j].checkInDate <= i << targetRent[j].checkOutDate){
            checkDate[0] = targetRent[j].checkInDate;
            checkDate[1] = targetRent[j].checkOutDate;
          }
        }
      }
    }
    return checkDate;
  }
}