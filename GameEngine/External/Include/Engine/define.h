#pragma once

#define SAFE_DELETE(p) if(p) {delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p) {p->Release();}

#define SINGLETON(type)	private:\
						static type* m_Inst;\
						public:\
						static type* GetInst()\
						{\
							if(!m_Inst)\
								m_Inst = new type;\
							return m_Inst;\
						}\
						static void DestroyInst()\
						{\
							SAFE_DELETE(m_Inst);\
						}
