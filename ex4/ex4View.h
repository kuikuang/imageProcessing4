
// ex4View.h : Cex4View 类的接口
//

#pragma once


class Cex4View : public CView
{
protected: // 仅从序列化创建
	Cex4View();
	DECLARE_DYNCREATE(Cex4View)

// 特性
public:
	Cex4Doc* GetDocument() const;
	void ShowDr(CDC* pDC, CString BmpName); //显示位图函数  
	void showEnhance(CDC* pDC);
											//添加成员变量  
	CString EntName;     //图像文件扩展名  
	CString BmpName;     //图像文件名称  
	CBitmap m_bitmap;    //创建位图对象  
	unsigned char *pixel;//存储像素数据数组
	unsigned char *zoom;//存储放缩后的像素数据
	short *temp; 
	unsigned char *newPicture; 
	unsigned char *ImageSize;
	int size;  //原始图片大小
	int key = 2;//放大/缩小倍数
	
	int sizeOfZoom;

	int Width;
	int Height;
	int newWidth; //放缩后的图片宽度
	int newHeight;//放缩后的图片高度
	int whiteWidth;//白色图片的宽度
	BITMAPFILEHEADER bfh;   //全局变量文件头  
	BITMAPINFOHEADER bih;   //全局变量信息头  
	RGBQUAD m_pPal;         //颜色表指针 
	int numPicture; //用来确定是否进行对比度增加 2是增加
// 操作 
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Cex4View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnEnhance();
};

#ifndef _DEBUG  // ex4View.cpp 中的调试版本
inline Cex4Doc* Cex4View::GetDocument() const
   { return reinterpret_cast<Cex4Doc*>(m_pDocument); }
#endif

