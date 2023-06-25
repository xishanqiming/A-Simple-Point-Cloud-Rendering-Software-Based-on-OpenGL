// CPointCloudView.cpp : CPointCloudView类的实现
//

#include "stdafx.h"
#include "PointCloud.h"

#include "PointCloudDoc.h"
#include "PointCloudView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PROCESS_ID_TRANSLATION	1    //ƽ�
#define PROCESS_ID_SCALE		2	 //����
#define PROCESS_ID_ROTATION		3	 //��ת
/////////////////////////////////////////////////////////////////////////////
// CPointCloudView

IMPLEMENT_DYNCREATE(CPointCloudView, CView)

BEGIN_MESSAGE_MAP(CPointCloudView, CView)
	//{{AFX_MSG_MAP(CPointCloudView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPointCloudView construction/destruction

CPointCloudView::CPointCloudView()
{
	m_pDC=NULL;
	m_bStatusbar=FALSE;
	m_bToolbar=FALSE;

	m_bShowPointCloud=FALSE;

	glTransX=glTransY=glTransZ=0;			//��ǰƽ�Ʋ���
	glCurTransX=glCurTransY=glCurTransZ=0;
	glRotX=glRotY=glRotZ=0.0;				//��ǰ��ת����
	glScaleX=glScaleY=glScaleZ=1.0;			//��ǰ����ϵ��
	glCurRotX=glCurRotY=glCurRotZ=0.0;
    m_bScale=FALSE;
	m_bRotation=FALSE;
	m_bMoving=FALSE;
	bInitOpenGL=FALSE;
	

}

CPointCloudView::~CPointCloudView()
{
}

BOOL CPointCloudView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style=WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CPointCloudView drawing

void CPointCloudView::OnDraw(CDC* pDC)
{
	CPointCloudDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	static BOOL bBusy=FALSE;
	if (bBusy)
	{
		return;
	}
	bBusy=TRUE;


	InitOpenGL();
	
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glTranslatef(pDoc->m_ptBoxCenter.x,pDoc->m_ptBoxCenter.y,pDoc->m_ptBoxCenter.z);

//	glTranslatef(0-glTransX,0-glTransY,0-glTransZ);	
	glTranslatef(0+glTransX+glCurTransX,0+glTransY+glCurTransY,0+glTransZ-glCurTransZ);	
	glRotatef(glCurRotX+glRotX,0.0, 1.0, 0.0);	//��y����ת
	glRotatef(glCurRotY+glRotY,1.0, 0.0, 0.0);	//��x����ת
	glRotatef(glCurRotZ+glRotZ,0.0, 0.0, 1.0);	//��x����ת
	glScalef(glScaleX,glScaleY,glScaleZ);
	
	glTranslatef(-pDoc->m_ptBoxCenter.x,-pDoc->m_ptBoxCenter.y,-pDoc->m_ptBoxCenter.z);
	
 //	glMatrixMode(GL_MODELVIEW);
// 	glGetFloatv(GL_MODELVIEW_MATRIX,ModelViewMatrix);
// 	glTranslatef(0+glTransX,0+glTransY,0+glTransZ);	
	//��ͼ����
	mydraw();
	// ��ɻ���
	glFinish();
	glPopMatrix();

	SwapBuffers(wglGetCurrentDC());
//	glMatrixMode(GL_MODELVIEW);
//	glLoadMatrixf(ModelViewMatrix);
//	glMatrixMode(GL_PROJECTION);
//	glLoadMatrixf(ProMatrix);
	bBusy=FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CPointCloudView printing

BOOL CPointCloudView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPointCloudView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPointCloudView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CPointCloudView diagnostics

#ifdef _DEBUG
void CPointCloudView::AssertValid() const
{
	CView::AssertValid();
}

void CPointCloudView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPointCloudDoc* CPointCloudView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPointCloudDoc)));
	return (CPointCloudDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPointCloudView message handlers

void CPointCloudView::mydraw()
{

	CPointCloudDoc*pDoc=GetDocument();
	ASSERT_VALID(pDoc);
	
	if (pDoc->m_ptVertexList.empty())
		return;

	


	glColor3f(1.0 , 1.0  , 1.0 );//	vector<POINT3D>::iterator pItPoint3d;
	vector<Point3D>::iterator pItPoint3d;
	glBegin(GL_POINTS);
	for (pItPoint3d = pDoc->m_ptVertexList.begin() ; 
	pItPoint3d != pDoc->m_ptVertexList.end() ; ++pItPoint3d)
		if (pItPoint3d->w > 0)
			glVertex3f(pItPoint3d->x , pItPoint3d->y ,pItPoint3d->z);
    glEnd();

	glBegin(GL_LINES);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(pDoc->m_ptBoxCenter.x,pDoc->m_ptBoxCenter.y,pDoc->m_ptBoxCenter.z);
		glVertex3f(pDoc->m_dbDistance,0.0, 0.0);
		glColor3f(0.0,1.0,0.0);
		glVertex3f(pDoc->m_ptBoxCenter.x,pDoc->m_ptBoxCenter.y,pDoc->m_ptBoxCenter.z);
		glVertex3f(0.0,pDoc->m_dbDistance, 0.0);
		glColor3f(0.0,0.0,1.0);
		glVertex3f(pDoc->m_ptBoxCenter.x,pDoc->m_ptBoxCenter.y,pDoc->m_ptBoxCenter.z);
		glVertex3f(0.0,0.0,pDoc->m_dbDistance);
	glEnd();
	glColor3f(1.0,1.0,1.0);

}

BOOL CPointCloudView::PiexlFormat()
{

	static PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,
			24,
			0,0,0,0,0,0,
			0,
			0,
			0,
			0,0,0,0,
			32,
			0,
			0,
			PFD_MAIN_PLANE,
			0,
			0,0,0
	};
	int nPixelFormat;


	if ( (nPixelFormat =ChoosePixelFormat(m_pDC->GetSafeHdc(),&pfd)) == 0 )
	{
		MessageBox("ChoosePixelFormat failed");
		return FALSE;
	}
	if (SetPixelFormat(m_pDC->GetSafeHdc(),nPixelFormat,&pfd) == FALSE)
	{
		MessageBox("SetPixelFormat failed");
		return FALSE;
	}
	return TRUE;
}


void CPointCloudView::InitRC()
{
	//�������ظ�ʽ
	PIXELFORMATDESCRIPTOR pfd;
	int n;
	m_pDC=new CClientDC(this);
	ASSERT(m_pDC!=NULL);
	if (!PiexlFormat())
	{
		return;
	}
	//�������ظ�ʽ
	n=::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(),n,sizeof(pfd),&pfd);

	
	
	
	GetClientRect(&m_rectOld);
	m_hRC=wglCreateContext(m_pDC->m_hDC);

	//�ӵ㡢�ӿ�����
	wglMakeCurrent(m_pDC->m_hDC,m_hRC);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


}

int CPointCloudView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	InitRC();		//�����Ⱦ������ʹ��
	
	return 0;
}



void CPointCloudView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	if (cy >0)
	{
		m_rectOld.right=cx;
		m_rectOld.bottom=cy;
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0,0,cx,cy);
	
	}
	
	RedrawWindow();//��ʾ����
	
}

void CPointCloudView::OnDestroy() 
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here

	wglDeleteContext(m_hRC);
	if (m_pDC)							//ɾ������Ⱦ������󶨵��豸
	{
		delete m_pDC;
	}
	
}

BOOL CPointCloudView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CView::OnEraseBkgnd(pDC);
}

void CPointCloudView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPointCloudDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// if(MK_MBUTTON*&&!MK_LBUTTON&&!MK_RBUTTON&&!MK_CONTROL&&!MK_SHIFT*/)Ϊʲô���У�����
	//����м�           ��ס�϶�            ƽ��
	if (m_bMoving)
	{
		
		glCurTransX=(GLfloat)(glDownX-point.x)/m_rectOld.Width()*2*pDoc->m_dbDistance;
		glCurTransY=(GLfloat)(glDownY-point.y)/m_rectOld.Height()*2*pDoc->m_dbDistance;
		Invalidate(FALSE);//��ʾ����
	}

	if (m_bRotation)
	{
		glCurRotX=(double)(glDownX-point.x)*360/(double)m_rectOld.Width();
		glCurRotY=(double)(glDownY-point.y)*360/(double)m_rectOld.Height();
		Invalidate(FALSE);//��ʾ����
	}
	
	CView::OnMouseMove(nFlags, point);
}

void CPointCloudView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//��ת
	m_bRotation=TRUE;
	glDownX=point.x;
	glDownY=point.y;
	glDownZ=0;
	CView::OnLButtonDown(nFlags, point);
}

void CPointCloudView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPointCloudDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

 	m_bRotation=FALSE;

	glCurRotX=double(glDownX-point.x)*360.0/(double)m_rectOld.Width();
	glCurRotY=double(glDownY-point.y)*360.0/(double)m_rectOld.Height();
	//ת��Ϊ�Ƕ�
//  	glMatrixMode(GL_PROJECTION);
// 	glLoadIdentity();
//   	glTranslatef(pDoc->m_ptBoxCenter.x,pDoc->m_ptBoxCenter.y,pDoc->m_ptBoxCenter.z);
//   	glRotatef(glCurRotX,0.0, 1.0, 0.0);	//��y����ת
//   	glRotatef(glCurRotY,1.0, 0.0, 0.0);	//��x����ת
//   	glTranslatef(-pDoc->m_ptBoxCenter.x,-pDoc->m_ptBoxCenter.y,-pDoc->m_ptBoxCenter.z);
// 	glGetFloatv(GL_PROJECTION,ProMatrix);
	glRotX+=glCurRotX;
	glRotY+=glCurRotY;
	glRotZ+=glCurRotZ;
	glCurRotX=0;
	glCurRotY=0;

	
	Invalidate(FALSE);
	

	CView::OnLButtonUp(nFlags, point);
}

void CPointCloudView::OnMButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	//����м�  ��ס�϶�   ƽ��

	m_bMoving=!m_bMoving;
	if (m_bMoving)
	{
		
		glDownX=point.x;
		glDownY=point.y;
		glDownZ=0;
	}
	
	
	CView::OnMButtonDown(nFlags, point);
}

void CPointCloudView::OnMButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CPointCloudDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (m_bMoving)
	{

		glCurTransX=(GLfloat)(glDownX-point.x)/m_rectOld.Width()*2*pDoc->m_dbDistance;
		glCurTransY=(GLfloat)(glDownY-point.y)/m_rectOld.Height()*2*pDoc->m_dbDistance;
		glTransX+=glCurTransX;
		glTransY+=glCurTransY;
		glTransZ+=glCurTransZ;
// 		glMatrixMode(GL_MODELVIEW);
// 		glTranslatef(glTransX,glTransY,glTransZ);
//		glPushMatrix();//��ջ
		glCurTransX=0;
		glCurTransY=0;
		glCurTransZ=0;
		m_bMoving=FALSE;
		Invalidate(FALSE);
	}
	CView::OnMButtonUp(nFlags, point);
}

BOOL CPointCloudView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	glScaleX=glScaleY=glScaleZ+=(GLfloat)zDelta/(GLfloat)m_rectOld.Height();

	

	Invalidate(FALSE);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CPointCloudView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
// 	m_bRotation=TRUE;
// 	glDownX=point.x;
// 	glDownY=point.y;
// 	glDownZ=0;
	
	CView::OnRButtonDown(nFlags, point);
}

void CPointCloudView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CView::OnRButtonUp(nFlags, point);
}


void CPointCloudView::ResetViewport()
{

}

void CPointCloudView::InitOpenGL()
{
	CPointCloudDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-pDoc->m_dbDistance,pDoc->m_dbDistance,
		-pDoc->m_dbDistance,pDoc->m_dbDistance,
		-pDoc->m_dbDistance,pDoc->m_dbDistance
		);
	//����ɫ Ĭ��Ϊ��ɫ
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glColor3f( 1.0, 1.0, 1.0);
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//������潫�����ÿ�
	
	gluLookAt(0,	0,		-pDoc->m_dbDistance,		0,		0,		0,		0.0, 1.0, 0.0);

	bInitOpenGL=TRUE;

}
