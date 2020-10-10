
// GBKUTF8ConvertDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "GBKUTF8Convert.h"
#include "GBKUTF8ConvertDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGBKUTF8ConvertDlg 对话框

CGBKUTF8ConvertDlg::CGBKUTF8ConvertDlg(CWnd* pParent /*=nullptr*/) : CDialog(IDD_GBKUTF8CONVERT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGBKUTF8ConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
}

BEGIN_MESSAGE_MAP(CGBKUTF8ConvertDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_GBKTOUTF8, &CGBKUTF8ConvertDlg::OnBnClickedButtonGbkToUtf8)
	ON_BN_CLICKED(IDC_BUTTON_UTF8TOGBK, &CGBKUTF8ConvertDlg::OnBnClickedButtonUtf8ToGbk)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CGBKUTF8ConvertDlg 消息处理程序

BOOL CGBKUTF8ConvertDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowText(_T("GBK文件和UTF8文件相互转换工具 37792738@qq.com"));
	SetDlgItemText(IDC_DESC, _T("把需要转换的文件拖放到下面(支持整个文件夹)......"));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGBKUTF8ConvertDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGBKUTF8ConvertDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//boost库引用文件  
#include <boost/locale.hpp>
#include <string>
#include <fstream>
#include <filesystem>

#include <algorithm>
#include <cctype>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <set>

template<typename = void>
inline std::string gbk_to_utf8(const std::string & s)
{
	return boost::locale::conv::between(s, "utf8", "gbk");
}

template<typename = void>
inline std::string utf8_to_gbk(const std::string & s)
{
	return boost::locale::conv::between(s, "gbk", "utf8");
}

template<typename = void>
inline std::string gbk_to_utf8(const char * s)
{
	return boost::locale::conv::between(s, "utf8", "gbk");
}

template<typename = void>
inline std::string utf8_to_gbk(const char * s)
{
	return boost::locale::conv::between(s, "gbk", "utf8");
}


#include <string>

namespace utf8_validator {

/// State that represents a valid utf8 input sequence
static unsigned int const utf8_accept = 0;
/// State that represents an invalid utf8 input sequence
static unsigned int const utf8_reject = 1;

/// Lookup table for the UTF8 decode state machine
static uint8_t const utf8d[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
  8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
  0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
  0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
  0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
  1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
  1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
  1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
};

/// Decode the next byte of a UTF8 sequence
/**
 * @param [out] state The decoder state to advance
 * @param [out] codep The codepoint to fill in
 * @param [in] byte The byte to input
 * @return The ending state of the decode operation
 */
inline uint32_t decode(uint32_t * state, uint32_t * codep, uint8_t byte) {
  uint32_t type = utf8d[byte];

  *codep = (*state != utf8_accept) ?
    (byte & 0x3fu) | (*codep << 6) :
    (0xff >> type) & (byte);

  *state = utf8d[256 + *state*16 + type];
  return *state;
}

/// Provides streaming UTF8 validation functionality
class validator {
public:
    /// Construct and initialize the validator
    validator() : m_state(utf8_accept),m_codepoint(0) {}

    /// Advance the state of the validator with the next input byte
    /**
     * @param byte The byte to advance the validation state with
     * @return Whether or not the byte resulted in a validation error.
     */
    bool consume (uint8_t byte) {
        if (utf8_validator::decode(&m_state,&m_codepoint,byte) == utf8_reject) {
            return false;
        }
        return true;
    }

    /// Advance validator state with input from an iterator pair
    /**
     * @param begin Input iterator to the start of the input range
     * @param end Input iterator to the end of the input range
     * @return Whether or not decoding the bytes resulted in a validation error.
     */
    template <typename iterator_type>
    bool decode (iterator_type begin, iterator_type end) {
        for (iterator_type it = begin; it != end; ++it) {
            unsigned int result = utf8_validator::decode(
                &m_state,
                &m_codepoint,
                static_cast<uint8_t>(*it)
            );

            if (result == utf8_reject) {
                return false;
            }
        }
        return true;
    }

    /// Return whether the input sequence ended on a valid utf8 codepoint
    /**
     * @return Whether or not the input sequence ended on a valid codepoint.
     */
    bool complete() {
        return m_state == utf8_accept;
    }

    /// Reset the validator to decode another message
    void reset() {
        m_state = utf8_accept;
        m_codepoint = 0;
    }
private:
    uint32_t    m_state;
    uint32_t    m_codepoint;
};

/// Validate a UTF8 string
/**
 * convenience function that creates a validator, validates a complete string
 * and returns the result.
 */
inline bool validate(std::string const & s) {
    validator v;
    if (!v.decode(s.begin(),s.end())) {
        return false;
    }
    return v.complete();
}

} // namespace utf8_validator

std::set<std::string> filter_types
{
	"dll",
	"lib",
	"pdb",
	"exp",
	"exe",
	"rar",
	"zip",
	"suo",
	"db",
	"db-shm",
	"db-wal",
	"ico",
	"rc2",
	"aps",
	"doc",
	"docx",
	"csv",
	"pdf",
	"msu",
	"msi",
	"bmp",
	"png",
	"jpg",
	"pch",
	"ipch",
	"obj",
	"idb",
	"lastbuildstate",
	"class",
	"o",
	"lo",
	"la",
	"al",
	"libs",
	"so",
	"a",
	"pyc",
	"pyo",
	"rej",
	"swp",
	"suo",
	"user",
	"userosscache",
	"docstates",
	"userprefs",
	"ilk",
	"meta",
	"obj",
	"pch",
	"pdb",
	"pgc",
	"pgd",
	"rsp",
	"sbr",
	"tlb",
	"tli",
	"tlh",
	"tmp",
	"tmp_proj",
	"log",
	"vspscc",
	"vssscc",
	"pidb",
	"svclog",
	"scc",
	"ipch",
	"aps",
	"ncb",
	"opendb",
	"opensdf",
	"sdf",
	"cachefile",
	"db",
	"opendb",
	"psess",
	"vsp",
	"vspx",
	"sap",
	"tlog",
	"idb",
	"iobj",
	"ipdb",
	"class",
	"target",
};

void CGBKUTF8ConvertDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHAR filePath[MAX_PATH]{ 0 };
	INT Num = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	if (Num > 0)
	{
		for (INT i = 0; i < Num; i++)
		{
			DragQueryFile(hDropInfo, i, filePath, sizeof(filePath));

			CStringA apath = CStringA(filePath);
			std::string path = (LPCSTR)(apath);

			auto add_to_list = [this](std::string path)
			{
				auto pos = path.find_last_of('.');
				if (pos != std::string::npos)
				{
					std::string type = path.substr(pos + 1);
					std::transform(type.begin(), type.end(), type.begin(),
						[](unsigned char c) { return std::tolower(c); });
					if (filter_types.find(type) != filter_types.end())
						return;
				}
				m_List.SetCheck(m_List.AddString(CString(path.data())), 1);
			};

			if (std::filesystem::is_regular_file(path))
			{
				add_to_list(path);
			}
			else if (std::filesystem::is_directory(path))
			{
				for (auto& entry : std::filesystem::recursive_directory_iterator(path))
				{
					if (std::filesystem::is_regular_file(entry))
					{
						add_to_list(entry.path().string());
					}
				}
			}
		}
	}
	DragFinish(hDropInfo);

	CDialog::OnDropFiles(hDropInfo);
}

void CGBKUTF8ConvertDlg::OnBnClickedButtonGbkToUtf8()
{
	int count = m_List.GetCount();
	for (int i = count - 1; i >= 0; --i)
	{
		if (m_List.GetCheck(i) == 1)
		{
			CString fpath;
			m_List.GetText(i, fpath);
			CStringA apath(fpath);
			std::string path = apath.GetBuffer();

			std::string str;
			{
				std::fstream file{ path,std::ios::in | std::ios::out | std::ios::binary };
				str.resize(std::filesystem::file_size(path));
				file.read(str.data(), str.size());
				file.close();
			}
			// 如果文件内容本来是UTF8的，就不能再转换了
			if (!utf8_validator::validate(str))
			{
				str = gbk_to_utf8(str);
				{
					std::fstream file{ path,std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc };
					file.write(str.data(), str.size());
					file.close();
				}
			}
			m_List.DeleteString(i);
		}
	}
}


void CGBKUTF8ConvertDlg::OnBnClickedButtonUtf8ToGbk()
{
	int count = m_List.GetCount();
	for (int i = count - 1; i >= 0; --i)
	{
		if (m_List.GetCheck(i) == 1)
		{
			CString fpath;
			m_List.GetText(i, fpath);
			CStringA apath(fpath);
			std::string path = apath.GetBuffer();

			std::string str;
			{
				std::fstream file{ path,std::ios::in | std::ios::out | std::ios::binary };
				str.resize(std::filesystem::file_size(path));
				file.read(str.data(), str.size());
				file.close();
			}
			// 如果文件内容本来是UTF8的，才能执行转换
			if (utf8_validator::validate(str))
			{
				str = utf8_to_gbk(str);
				{
					std::fstream file{ path,std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc };
					file.write(str.data(), str.size());
					file.close();
				}
			}
			m_List.DeleteString(i);
		}
	}// TODO: 在此添加控件通知处理程序代码
}
