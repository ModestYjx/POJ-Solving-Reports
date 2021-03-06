/*
	Author:     Exp
	Date:       2017-11-30
	Code:       POJ 1083
	Problem:    Moving Tables
	URL:		http://poj.org/problem?id=1083
*/

/*
	题意分析：
	 酒店有400个房间，如下编号对称分布在一条走廊两侧
	 1 3 5 ... 397 399
	    这里是走廊
	 2 4 6 ... 398 400

	 现在要从房间s->t搬桌子，有N组房间需要搬桌子，每搬一次10分钟(不论两个房间相隔多远).
	 对于某组房间，搬桌子期间 s->t 之间的走廊会被占用，
	 而其他组房间若没有使用到占用的走廊，则可同时搬，否则要等待.
	 特别地，相对的两间房，共用一段走廊, 即若房间3的走廊被占用了，等同于房间4的走廊被占用了.

	 给定N组需要搬的桌子，求最小可以搬完的时间.


	解题思路:
	 感觉这题有点类似于多线程的同步互斥场景问题.

	 ① 由于相对的两间房共用同一段走廊，为了方便处理问题，可以把所有奇数房间转换成偶数房间，反之亦可.
	    如 1->6 可等价转换成 2->6， 尔后只需要关注一侧的房间即可（共200间）
	 ② 根据房间号对走廊进行分割，可分割成200段走廊. 在从房间s->t搬桌子期间，对所使用到的每段走廊计数+1
	 ③ 所有桌子搬完后，统计每段走廊的计数值（计数值代表这段走廊总共需要被占用的次数）,
	    由于题目并不考虑搬动期间的移动过程因素(类比多线程的事务锁)，
		因此最大的一个计数值*10分钟 就是所求的最小搬完时间（因为只要走廊被占着，就只能在下一次再搬，不能同时搬）

*/

#include <iostream>
using namespace std;

const static int ROOM_NUM = 400;	// 最大房号
const static int TIME_UNIT = 10;	// 时间单位

/* 
 * 把偶数房号转换成奇数房号
 * @param even 偶数房号
 * return 奇数房号
 */
int toOdd(int even);

/* 
 * 问题求解
 */
void solve(void);


int main(void) {
	int testCase = 0;
	cin >> testCase;
	for(int t = 0; t < testCase; t++) {
		solve();
	}

	//system("pause");
	return 0;
}


int toOdd(int even) {
	return (even % 2 == 0 ? even - 1 : even);
}


void solve(void) {
	int useCnt[ROOM_NUM] = { 0 };		// 每个房间前的走廊被使用的次数
	int maxUseCnt = 0;					// 被使用最多的次数

	int moveCnt = 0;					// 需要搬动的桌子组数
	cin >> moveCnt;
	int* fromRooms = new int[moveCnt];	// 起点房间集
	int* toRooms = new int[moveCnt];	// 终点房间集
	for(int i = 0; i < moveCnt; i++) {
		int from, to;
		cin >> from >> to;

		// 使房号小的在前面
		fromRooms[i] = (from <= to ? from : to);
		toRooms[i] = (from > to ? from : to);

		// 把房号全部转换成奇数
		fromRooms[i] = toOdd(fromRooms[i]);	
		toRooms[i] = toOdd(toRooms[i]);

		// 相关房间前占用的走廊被使用次数+1
		for(int roomId = fromRooms[i]; roomId <= toRooms[i]; roomId += 2) {
			useCnt[roomId]++;
			if(maxUseCnt < useCnt[roomId]) {
				maxUseCnt = useCnt[roomId];	// 登记最大的使用次数
			}
		}
	}
	delete[] fromRooms;
	delete[] toRooms;

	// 计算最小的使用时间
	maxUseCnt = (maxUseCnt <= 0 || maxUseCnt > moveCnt ? moveCnt : maxUseCnt);
	int minUsedTime = maxUseCnt * TIME_UNIT;
	cout << minUsedTime << endl;
}
