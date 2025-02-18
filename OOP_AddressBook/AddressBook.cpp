#include "CUserInterface.h"
#include "CLinkedList.h"
#include "PrintListObserver.h"
#include "ListInfoObserver.h"

int main(void) {

	CLinkedList& list = CLinkedList::GetInstance();
	CUserInterface ui(list);

	PrintListObserver listview(list);
	ListInfoObserver infoview(list);

	ui.AddObserver(&listview);
	ui.AddObserver(&infoview);

	ui.EventLoopRun();

	return 0;
}