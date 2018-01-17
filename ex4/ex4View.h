
// ex4View.h : Cex4View ��Ľӿ�
//

#pragma once


class Cex4View : public CView
{
protected: // �������л�����
	Cex4View();
	DECLARE_DYNCREATE(Cex4View)

// ����
public:
	Cex4Doc* GetDocument() const;
	void ShowDr(CDC* pDC, CString BmpName); //��ʾλͼ����  
	void showEnhance(CDC* pDC);
											//��ӳ�Ա����  
	CString EntName;     //ͼ���ļ���չ��  
	CString BmpName;     //ͼ���ļ�����  
	CBitmap m_bitmap;    //����λͼ����  
	unsigned char *pixel;//�洢������������
	unsigned char *zoom;//�洢���������������
	short *temp; 
	unsigned char *newPicture; 
	unsigned char *ImageSize;
	int size;  //ԭʼͼƬ��С
	int key = 2;//�Ŵ�/��С����
	
	int sizeOfZoom;

	int Width;
	int Height;
	int newWidth; //�������ͼƬ���
	int newHeight;//�������ͼƬ�߶�
	int whiteWidth;//��ɫͼƬ�Ŀ��
	BITMAPFILEHEADER bfh;   //ȫ�ֱ����ļ�ͷ  
	BITMAPINFOHEADER bih;   //ȫ�ֱ�����Ϣͷ  
	RGBQUAD m_pPal;         //��ɫ��ָ�� 
	int numPicture; //����ȷ���Ƿ���жԱȶ����� 2������
// ���� 
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~Cex4View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // ex4View.cpp �еĵ��԰汾
inline Cex4Doc* Cex4View::GetDocument() const
   { return reinterpret_cast<Cex4Doc*>(m_pDocument); }
#endif

