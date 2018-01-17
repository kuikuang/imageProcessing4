
// ex4View.cpp : Cex4View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
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

// Cex4View 构造/析构

Cex4View::Cex4View()
{
	// TODO: 在此处添加构造代码

}

Cex4View::~Cex4View()
{
}

BOOL Cex4View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Cex4View 绘制

void Cex4View::OnDraw(CDC* pDC)
{
	Cex4Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here  
	if (!pDoc) return;
	if (EntName.Compare(_T("dr")) == 0)      //bmp格式    
	{
		
			ShowDr(pDC, BmpName);
		
	
			
	}
	// TODO: 在此处为本机数据添加绘制代码

	// TODO: 在此处为本机数据添加绘制代码
}


// Cex4View 打印


void Cex4View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cex4View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Cex4View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Cex4View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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
	
#pragma pack(2)//必须得写，否则sizeof得不到正确的结果
		FILE *input = fopen(BmpName, "rb");//打开医学图像文件

		typedef unsigned char  BYTE;  //从这里开始定义BMP文件的文件结构
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
		fread(&w, sizeof(short), 1, input);//读宽度
		fread(&h, sizeof(short), 1, input);//读高度


		int height = h;
		int width = w;
		Width = w;
		Height = h;
		fseek(input, 16, 0);
		size = h*w;
		temp = new short[size];
		pixel = new unsigned char[size];

		for (int q = 0; q < size; q++)  //读像素数据
		{
			fread(&temp[size - 1 - q], sizeof(short), 1, input);
		}//相反

		int Cw = 2000;//窗位
		int Ww = 4100;//窗宽

		for (int i = 0; i < size; i++)//开窗显示算法
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

		//原始缩放系数是2 即先缩小一半

		newHeight = Height / key;     //由于原图像过大 所以显示前先缩小一半
		newWidth = Width / key;


		newWidth = (newWidth + 3) / 4 * 4;  //bmp文件规定扫描的字节数必须是4的整倍数 所以如果宽度不满4补1
		sizeOfZoom = newHeight*newWidth;
		zoom = new unsigned char[newHeight*newWidth];
		for (int i = 0; i < newHeight; i++)    //缩小算法 就是根据缩小的倍数将原来的像素点映射到缩放后的像素点
		{
			for (int j = 0; j < newWidth; j++)
			{

				if (j > Width / key - 1)  //宽度补1的地方补0
				{
					zoom[i*newWidth + j] = 0;
				}

				else
				{
					int c = (i * key + key - 1)*Width + j * key + key - 1;//映射

					zoom[i*newWidth + j] = pixel[c];
				}

			}
		}
		if (numPicture == 2)    //进行对比度增强操作
		{

			for (int i = 0; i < newHeight; i++)
			{
				int s1 = 50; int s2 = 150; int t1 = 60; int t2 = 100;  //采用线性拉伸算法  突出感兴趣的目标或者灰度区间，相对抑制那些不感兴趣的灰度区域
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



		BITMAPFILEHEADER fileHeader;   //给bmp头赋值

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

		//生成颜色表
		for (int iiii = 0; iiii < 256; iiii++) {
			buffer[0 + 4 * iiii] = (byte)iiii;   //Blue
			buffer[1 + 4 * iiii] = (byte)iiii;   //Green
			buffer[2 + 4 * iiii] = (byte)iiii;   //Red
			buffer[3 + 4 * iiii] = (byte)0xFF;   //保留值
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


		FILE *white = fopen("white.bmp", "wb");  //为了显示效果 生成一张纯白图片
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
		//定义bitmap指针 调用函数LoadImage装载位图  

	
	HBITMAP m_hBitmap;
	m_hBitmap = (HBITMAP)LoadImage(NULL, "output.bmp", IMAGE_BITMAP, newWidth, newHeight,
		LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);


	if (m_bitmap.m_hObject)
	{
		m_bitmap.Detach();           //切断CWnd和窗口联系  
	}
	m_bitmap.Attach(m_hBitmap);      //将句柄HBITMAP m_hBitmap与CBitmap m_bitmap关联  

									 //边界  
	CRect rect;
	GetClientRect(&rect);

	//图片显示(x,y)起始坐标  
	int m_showX = 0;
	int m_showY = 0;
	int m_nWindowWidth = rect.right - rect.left;   //计算客户区宽度  
	int m_nWindowHeight = rect.bottom - rect.top;  //计算客户区高度  

												   //定义并创建一个内存设备环境DC  
	CDC dcBmp;
	if (!dcBmp.CreateCompatibleDC(pDC))   //创建兼容性的DC  
		return;
	BITMAP m_bmp;                          //临时bmp图片变量  
	m_bitmap.GetBitmap(&m_bmp);            //将图片载入位图中  
	CBitmap *pbmpOld = NULL;
	dcBmp.SelectObject(&m_bitmap);         //将位图选入临时内存设备环境  

										   //图片显示调用函数stretchBlt  
	if (key % 2 == 1)             //在显示缩放奇数倍时需要一些处理
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


	dcBmp.SelectObject(pbmpOld);           //恢复临时DC的位图  
	DeleteObject(&m_bitmap);               //删除内存中的位图  
	dcBmp.DeleteDC();                      //删除CreateCompatibleDC得到的图片DC  





	if (key % 2 == 1)          //  在显示缩放奇数倍时需要一些处理
	{         
	 
		HBITMAP m_hBitmapChange;
 
		CRect rect;
		GetClientRect(&rect);

	
		int m_showX = 0;
		int m_showY = 0;
		int m_nWindowWidth = rect.right - rect.left;   //计算客户区宽度  
		int m_nWindowHeight = rect.bottom - rect.top;  //计算客户区高度  
		m_hBitmapChange = (HBITMAP)LoadImage(NULL, "white.bmp", IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_CREATEDIBSECTION);

		if (m_bitmap.m_hObject) {
			m_bitmap.Detach();            //m_bitmap为创建的位图对象  
		}
		m_bitmap.Attach(m_hBitmapChange);
		//定义并创建一个内存设备环境  
		CDC dcBmp;
		if (!dcBmp.CreateCompatibleDC(pDC))   //创建兼容性的DC  
			return;
		BITMAP m_bmp;                          //临时bmp图片变量  
		m_bitmap.GetBitmap(&m_bmp);            //将图片载入位图中  
		CBitmap *pbmpOld = NULL;
		dcBmp.SelectObject(&m_bitmap);         //将位图选入临时内存设备环境  

											   //如果图片太大显示大小为固定640*640 否则显示原图大小  
		pDC->StretchBlt(0, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0,
			whiteWidth, newHeight, SRCCOPY);

		pDC->StretchBlt(whiteWidth * 3, 0, m_bmp.bmWidth, m_bmp.bmHeight, &dcBmp, 0, 0,
			whiteWidth, newHeight, SRCCOPY);

		//恢复临时DC的位图  
		dcBmp.SelectObject(pbmpOld);
	}
}


// Cex4View 诊断

#ifdef _DEBUG
void Cex4View::AssertValid() const
{
	CView::AssertValid();
}

void Cex4View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cex4Doc* Cex4View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cex4Doc)));
	return (Cex4Doc*)m_pDocument;
}
#endif //_DEBUG


// Cex4View 消息处理程序


void Cex4View::OnFileOpen()
{
	CString filter;
	filter = "所有文件(*.dr)|*.dr;|DR(*.dr)|*.dr|| ";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter, NULL);
	if (dlg.DoModal() == IDOK)
	{
		BmpName = dlg.GetPathName();     //获取文件路径名   如D:\pic\abc.bmp    
		EntName = dlg.GetFileExt();      //获取文件扩展名    
		EntName.MakeLower();
		numPicture = 1;//将文件扩展名转换为一个小写字符    
		Invalidate();                    //调用该函数就会调用OnDraw重绘画图    
	}
	// TODO: 在此添加命令处理程序代码
}


void Cex4View::OnZoomIn()//缩小函数
{
	key++;  //增加缩小倍数
	Invalidate();
}


void Cex4View::OnZoomOut()//放大函数
{
	if (key == 2)
	{
		AfxMessageBox("放大后大小超过显示范围，请重新选择操作", MB_OK, 0);
	}
	else
	{
		key--;//减少缩小倍数
		Invalidate();
	}

}


void Cex4View::OnEnhance()
{

	numPicture = 2;
	Invalidate();
}
