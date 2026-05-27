#include		"itemPackList.h"

cItemPackList	g_itemPackList;

void
cItemPackList::init()
{
	reset();

	cFILE	file;

	if (file.Open("data/itemPackList.dat","rb"))
	{
		file.Read(m_aPacks,sizeof(m_aPacks));
		file.Close();
	}
}

void
cItemPackList::reset()
{
	for (int i=0;i<dMAX_ITEM_PACK_COUNT;i++)
		m_aPacks[i].reset();
}