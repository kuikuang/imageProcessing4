
// ex4View.cpp : Cex4View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ex4.h"
#endif

#include "ex4Doc.h"
#include "ex4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cex4View

IMPLEMENT_DYNCREATE(Cex4View, CView)

BEGIN_MESSAGE_MAP(Cex4View, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cex4View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, &Cex4View::OnFileOpen)
	ON_COMMAND(ID_ZOOM_IN, &Cex4View::OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, &Cex4View::OnZoomOut)
	ON_COMMAND(ID_ENHANCE, &Cex4View::OnEnhance)
END_MESSAGE_MAP()

// Cex4View ����/����

Cex4View::Cex4View()
{
	// TODO: �ڴ˴���ӹ������

}

Cex4View::~Cex4View()
{
}

BOOL Cex4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Cex4View ����

void Cex4View::OnDraw(CDC* pDC)
{
	Cex4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here  
	if (!pDoc) return;
	if (EntName.Compare(_T("dr")) == 0)      //bmp��ʽ    
	{
		
			ShowDr(pDC, BmpName);
		
	
			
	}
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// Cex4View ��ӡ


void Cex4View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cex4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Cex4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void Cex4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void Cex4View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cex4View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void Cex4View::ShowDr(CDC* pDC, CString BmpName)
{
	
#pragma pack(2)//�����д������sizeof�ò�����ȷ�Ľ��
		FILE *input = fopen(BmpName, "rb");//��ҽѧͼ���ļ�

		typedef unsigned char  BYTE;  //�����￪ʼ����BMP�ļ����ļ��ṹ
		typedef unsigned short WORD;
		typedef unsigned long  DWORD;
		typedef long    LONG;
		typedef struct {
			WORD    bfType;
			DWORD   bfSize;
			WORD    bfReserved1;
			WORD    bfReserved2;
			DWORD   bfOffBits;
		} BITMAPFILEHEADER;

		typedef struct {
			DWORD      biSize;
			LONG       biWidth;
			LONG       biHeight;
			WORD       biPlanes;
			WORD       biBitCount;
			DWORD      biCompression;
			DWORD      biSizeImage;

			LONG       biXPelsPerMeter;
			LONG       biYPelsPerMeter;
			DWORD      biClrUsed;
			DWORD      biClrImportant;
		} BITMAPINFOHEADER;       

		short w;
		short h;
		fread(&w, sizeof(short), 1, input);//�����
		fread(&h, sizeof(short), 1, input);//���߶�


		int height = h;
		int width = w;
		Width = w;
		Height = h;
		fseek(input, 16, 0);
		size = h*w;
		temp = new short[size];
		pixel = new unsigned char[size];

		for (int q = 0; q < size; q++)  //����������
		{
			fread(&temp[size - 1 - q], sizeof(short), 1, input);
		}//�෴

		int Cw = 2000;//��λ
		int Ww = 4100;//����

		for (int i = 0; i < size; i++)//������ʾ�㷨
		{
			if (temp[i] <= (Cw - Ww / 2))
			{
				pixel[i] = 0;
			}
			else if (temp[i] >= Cw + Ww / 2)
			{
				pixel[i] = 255;
			}
			else
			{
				pixel[i] = ((temp[i] - Cw + Ww / 2) * 255) / Ww;
			}
		}

		//ԭʼ����ϵ����2 ������Сһ��

		newHeight = Height / key;     //����ԭͼ����� ������ʾǰ����Сһ��
		newWidth = Width / key;


		newWidth = (newWidth + 3) / 4 * 4;  //bmp�ļ��涨ɨ����ֽ���������4�������� ���������Ȳ���4��1
		sizeOfZoom = newHeight*newWidth;
		zoom = new unsigned char[newHeight*newWidth];
		for (int i = 0; i < newHeight; i++)    //��С�㷨 ���Ǹ�����С�ı�����ԭ�������ص�ӳ�䵽���ź�����ص�
		{
			for (int j = 0; j < newWidth; j++)
			{

				if (j > Width / key - 1)  //��Ȳ�1�ĵط���0
				{
					zoom[i*newWidth + j] = 0;
				}

				else
				{
					int c = (i * key + key - 1)*Width + j * key + key - 1;//ӳ��

					zoom[i*newWidth + j] = pixel[c];
				}

			}
		}
		if (numPicture == 2)    //���жԱȶ���ǿ����
		{

			for (int i = 0; i < newHeight; i++)
			{
				int s1 = 50; int s2 = 150; int t1 = 60; int t2 = 100;  //�������������㷨  ͻ������Ȥ��Ŀ����߻Ҷ����䣬���������Щ������Ȥ�ĻҶ�����
				for (int j = 0; j < newWidth; j++)
				{
					

					numPicture = 1;
					if ((zoom[i*newWidth + j]) < s1)         
					{
						zoom[i*newWidth + j] = zoom[i*newWidth + j] * t1 / s1;
					}
					else
						if (s1<=zoom[i*newWidth + j]<=s2)
						{
							zoom[i*newWidth + j] = (zoom[i*newWidth + j] - s1) * (t2-t1) / (s2-s1) + t1;
						}
						else
							zoom[i*newWidth + j] =( zoom[i*newWidth + j] -s2)*(255-t2)/(255-s2)+t2;
				
				

				}
			}
		}



		BITMAPFILEHEADER fileHeader;   //��bmpͷ��ֵ

		fileHeader.bfType = 0x4D42;
		fileHeader.bfReserved1 = 0;
		fileHeader.bfReserved2 = 0;
		fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + newHeight*newWidth;
		fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		// Part.2 Create Bitmap Info Header
		BITMAPINFOHEADER bitmapHeader = { 0 };

		bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapHeader.biHeight = newHeight;
		bitmapHeader.biWidth = newWidth;

		bitmapHeader.biPlanes = 1;
		bitmapHeader.biBitCount = 8;


		bitmapHeader.biSizeImage = newHeight*newWidth;
		bitmapHeader.biCompression = 0; //BI_RGB


										// Write to file

		FILE *output = fopen("output.bmp", "wb");

		byte buffer[256 * 4];

		//������ɫ��
		for (int iiii = 0; iiii < 256; iiii++) {
			buffer[0 + 4 * iiii] = (byte)iiii;   //Blue
			buffer[1 + 4 * iiii] = (byte)iiii;   //Green
			buffer[2 + 4 * iiii] = (byte)iiii;   //Red
			buffer[3 + 4 * iiii] = (byte)0xFF;   //����ֵ
		}


		if (output == NULL)
		{
			printf("Cannot open file!\n");
		}
		else
		{
			fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, output);
			fwrite(&bitmapHeader, sizeof(BITMAPINFOHEADER), 1, output);
			fwrite(buffer, 1024, 1, output);
			fwrite(zoom, newHeight*newWidth, 1, output);
			fclose(output);
		}


		FILE *white = fopen("white.bmp", "wb");  //Ϊ����ʾЧ�� ����һ�Ŵ���ͼƬ
		int whiteWidth = newWidth / 2;
		whiteWidth = (whiteWidth + 3) / 4 * 4;
		unsigned char*de = new unsigned char[whiteWidth*newHeight];

		for (int i = 0; i < whiteWidth*newHeight; i++)
		{
			de[i] = 255;
		}
		fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + whiteWidth*newHeight;
		bitmapHeader.biWidth = whiteWidth;
		bitmapHeader.biSizeImage = newHeight*whiteWidth;
		fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, white);
		fwrite(&bitmapHeader, sizeof(BITMAPINFOHEADER), 1, white);
		fwrite(buffer, 1024, 1, white);
		fwrite(de, whiteWidth*newHeight, 1, white);
		fclose(white);
		//����bitmapָ�� ���ú���LoadImageװ��λͼ  

	
	HBITMAP m_hBitmap;
	m_hBitmap = (HBITMAP)LoadImage(NULL, "output.bmp", IMAGE_BITMAP, newWidth, newHeight,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);


	if (m_bitmap.m_hObject)
	{
		m_bitmap.Detach();           //�ж�CWnd�ʹ�����ϵ  
	}
	m_bitmap.Attach(m_hBitmap);      //�����HBITMAP m_hBitmap��CBitmap m_bitmap����  

									 //�߽�  
	CRect rect;
	GetClientRect(&rect);

	//ͼƬ��ʾ(x,y)��ʼ����  
	int m_showX = 0;
	int m_showY = 0;
	int m_nWindowWidth = rect.right - rect.left;   //����ͻ������  
	int m_nWindowHeight = rect.bottom - rect.top;  //����ͻ����߶�  

												   //���岢����һ���ڴ��豸����DC  
	CDC dcBmp;
	if (!dcBmp.CreateCompatibleDC(pDC))   //���������Ե�DC  
		return;
	BITMAP m_bmp;                          //��ʱbmpͼƬ����  
	m_bitmap.GetBitmap(&m_bmp);            //��ͼƬ����λͼ��  
	CBitmap *pbmpOld = NULL;
	dcBmp.SelectObject(&m_bitmap);         //��λͼѡ����ʱ�ڴ��豸����  

										   //ͼƬ��ʾ���ú���stretchBlt  
	if (key % 2 == 1)             //����ʾ����������ʱ��ҪһЩ����
	{

		pDC->StretchBlt(0, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0,
			newWidth, newHeight, SRCCOPY);

		pDC->StretchBlt(newWidth, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0,
			newWidth, newHeight, SRCCOPY);
	}
	else
	{
		pDC->StretchBlt(newWidth / 2, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0,
			newWidth, newHeight, SRCCOPY);
	}


	dcBmp.SelectObject(pbmpOld);           //�ָ���ʱDC��λͼ  
	DeleteObject(&m_bitmap);               //ɾ���ڴ��е�λͼ  
	dcBmp.DeleteDC();                      //ɾ��CreateCompatibleDC�õ���ͼƬDC  





	if (key % 2 == 1)          //  ����ʾ����������ʱ��ҪһЩ����
	{         
	 
		HBITMAP m_hBitmapChange;
 
		CRect rect;
		GetClientRect(&rect);

	
		int m_showX = 0;
		int m_showY = 0;
		int m_nWindowWidth = rect.right - rect.left;   //����ͻ������  
		int m_nWindowHeight = rect.bottom - rect.top;  //����ͻ����߶�  
		m_hBitmapChange = (HBITMAP)LoadImage(NULL, "white.bmp", IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);

		if (m_bitmap.m_hObject) {
			m_bitmap.Detach();            //m_bitmapΪ������λͼ����  
		}
		m_bitmap.Attach(m_hBitmapChange);
		//���岢����һ���ڴ��豸����  
		CDC dcBmp;
		if (!dcBmp.CreateCompatibleDC(pDC))   //���������Ե�DC  
			return;
		BITMAP m_bmp;                          //��ʱbmpͼƬ����  
		m_bitmap.GetBitmap(&m_bmp);            //��ͼƬ����λͼ��  
		CBitmap *pbmpOld = NULL;
		dcBmp.SelectObject(&m_bitmap);         //��λͼѡ����ʱ�ڴ��豸����  

											   //���ͼƬ̫����ʾ��СΪ�̶�640*640 ������ʾԭͼ��С  
		pDC->StretchBlt(0, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0,
			whiteWidth, newHeight, SRCCOPY);

		pDC->StretchBlt(whiteWidth * 3, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0,
			whiteWidth, newHeight, SRCCOPY);

		//�ָ���ʱDC��λͼ  
		dcBmp.SelectObject(pbmpOld);
	}
}


// Cex4View ���

#ifdef _DEBUG
void Cex4View::AssertValid() const
{
	CView::AssertValid();
}

void Cex4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cex4Doc* Cex4View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cex4Doc)));
	return (Cex4Doc*)m_pDocument;
}
#endif //_DEBUG


// Cex4View ��Ϣ�������


void Cex4View::OnFileOpen()
{
	CString filter;
	filter = "�����ļ�(*.dr)|*.dr;|DR(*.dr)|*.dr|| ";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		BmpName = dlg.GetPathName();     //��ȡ�ļ�·����   ��D:\pic\abc.bmp    
		EntName = dlg.GetFileExt();      //��ȡ�ļ���չ��    
		EntName.MakeLower();
		numPicture = 1;//���ļ���չ��ת��Ϊһ��Сд�ַ�    
		Invalidate();                    //���øú����ͻ����OnDraw�ػ滭ͼ    
	}
	// TODO: �ڴ���������������
}


void Cex4View::OnZoomIn()//��С����
{
	key++;  //������С����
	Invalidate();
}


void Cex4View::OnZoomOut()//�Ŵ���
{
	if (key == 2)
	{
		AfxMessageBox("�Ŵ���С������ʾ��Χ��������ѡ�����", MB_OK, 0);
	}
	else
	{
		key--;//������С����
		Invalidate();
	}

}


void Cex4View::OnEnhance()
{

	numPicture = 2;
	Invalidate();
}
