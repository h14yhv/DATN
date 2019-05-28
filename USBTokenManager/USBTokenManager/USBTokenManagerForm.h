#pragma once
#include "device.h"
#include "Common.h"
#include "string.h"
#include <vcclr.h>
#include "Md5.h"
#include "FormInfo.h"

using namespace std;
using namespace System::Runtime::InteropServices;
using namespace System;
using namespace System::Security::Cryptography;

extern DEVICE_DATA g_DeviceData;

namespace USBTokenManager {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for USBTokenManagerForm
	/// </summary>
	public ref class USBTokenManagerForm : public System::Windows::Forms::Form
	{
	public:
		USBTokenManagerForm(void)
		{
			InitializeComponent();


			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~USBTokenManagerForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  grbDevice;
	private: System::Windows::Forms::Button^  btnAuthenticate;
	protected:

	protected:



	private: System::Windows::Forms::Button^  btnSetPIN;

	private: System::Windows::Forms::Label^  lblPINCode;
	private: System::Windows::Forms::TextBox^  tbPINCode;


	private: System::Windows::Forms::ComboBox^  cbbListDevice;
	private: System::Windows::Forms::TextBox^  tbNewPINCode;


	private: System::Windows::Forms::Label^  lblNewPinCode;
	private: System::Windows::Forms::GroupBox^  grbSignature;

	private: System::Windows::Forms::Button^  btnReadSignature;
	private: System::Windows::Forms::TextBox^  tbReadSignature;

	private: System::Windows::Forms::Button^  btnBrowseFileName;

	private: System::Windows::Forms::TextBox^  tbFileName;

	private: System::Windows::Forms::Label^  lblFileName;

	private: System::Windows::Forms::ComboBox^  cbbListSignature;
	private: System::Windows::Forms::Button^  btnExit;
	private: System::Windows::Forms::Label^  lblSignatureWrite;

	private: System::Windows::Forms::Label^  lblSignatureRead;

	private: System::Windows::Forms::Button^  btnWriteSignature;
	private: System::Windows::Forms::TextBox^  tbWriteSignature;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(USBTokenManagerForm::typeid));
			this->grbDevice = (gcnew System::Windows::Forms::GroupBox());
			this->btnAuthenticate = (gcnew System::Windows::Forms::Button());
			this->btnSetPIN = (gcnew System::Windows::Forms::Button());
			this->lblPINCode = (gcnew System::Windows::Forms::Label());
			this->tbPINCode = (gcnew System::Windows::Forms::TextBox());
			this->cbbListDevice = (gcnew System::Windows::Forms::ComboBox());
			this->tbNewPINCode = (gcnew System::Windows::Forms::TextBox());
			this->lblNewPinCode = (gcnew System::Windows::Forms::Label());
			this->grbSignature = (gcnew System::Windows::Forms::GroupBox());
			this->lblSignatureWrite = (gcnew System::Windows::Forms::Label());
			this->lblSignatureRead = (gcnew System::Windows::Forms::Label());
			this->btnWriteSignature = (gcnew System::Windows::Forms::Button());
			this->tbWriteSignature = (gcnew System::Windows::Forms::TextBox());
			this->btnReadSignature = (gcnew System::Windows::Forms::Button());
			this->tbReadSignature = (gcnew System::Windows::Forms::TextBox());
			this->btnBrowseFileName = (gcnew System::Windows::Forms::Button());
			this->tbFileName = (gcnew System::Windows::Forms::TextBox());
			this->lblFileName = (gcnew System::Windows::Forms::Label());
			this->cbbListSignature = (gcnew System::Windows::Forms::ComboBox());
			this->btnExit = (gcnew System::Windows::Forms::Button());
			this->grbDevice->SuspendLayout();
			this->grbSignature->SuspendLayout();
			this->SuspendLayout();
			// 
			// grbDevice
			// 
			this->grbDevice->Controls->Add(this->btnAuthenticate);
			this->grbDevice->Controls->Add(this->btnSetPIN);
			this->grbDevice->Controls->Add(this->lblPINCode);
			this->grbDevice->Controls->Add(this->tbPINCode);
			this->grbDevice->Controls->Add(this->cbbListDevice);
			this->grbDevice->Controls->Add(this->tbNewPINCode);
			this->grbDevice->Controls->Add(this->lblNewPinCode);
			this->grbDevice->Location = System::Drawing::Point(12, 3);
			this->grbDevice->Name = L"grbDevice";
			this->grbDevice->Size = System::Drawing::Size(663, 178);
			this->grbDevice->TabIndex = 13;
			this->grbDevice->TabStop = false;
			this->grbDevice->Text = L"Device";
			// 
			// btnAuthenticate
			// 
			this->btnAuthenticate->Location = System::Drawing::Point(512, 64);
			this->btnAuthenticate->Name = L"btnAuthenticate";
			this->btnAuthenticate->Size = System::Drawing::Size(128, 32);
			this->btnAuthenticate->TabIndex = 11;
			this->btnAuthenticate->Text = L"Authenticate PIN";
			this->btnAuthenticate->UseVisualStyleBackColor = true;
			this->btnAuthenticate->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnAuthenticate_Click);
			// 
			// btnSetPIN
			// 
			this->btnSetPIN->Enabled = false;
			this->btnSetPIN->Location = System::Drawing::Point(512, 117);
			this->btnSetPIN->Name = L"btnSetPIN";
			this->btnSetPIN->Size = System::Drawing::Size(128, 30);
			this->btnSetPIN->TabIndex = 9;
			this->btnSetPIN->Text = L"Set PIN";
			this->btnSetPIN->UseVisualStyleBackColor = true;
			this->btnSetPIN->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnSetPIN_Click);
			// 
			// lblPINCode
			// 
			this->lblPINCode->AutoSize = true;
			this->lblPINCode->Location = System::Drawing::Point(14, 64);
			this->lblPINCode->Name = L"lblPINCode";
			this->lblPINCode->Size = System::Drawing::Size(67, 17);
			this->lblPINCode->TabIndex = 6;
			this->lblPINCode->Text = L"PIN Code";
			// 
			// tbPINCode
			// 
			this->tbPINCode->Location = System::Drawing::Point(144, 59);
			this->tbPINCode->Name = L"tbPINCode";
			this->tbPINCode->PasswordChar = '*';
			this->tbPINCode->Size = System::Drawing::Size(303, 22);
			this->tbPINCode->TabIndex = 8;
			// 
			// cbbListDevice
			// 
			this->cbbListDevice->FormattingEnabled = true;
			this->cbbListDevice->Location = System::Drawing::Point(17, 21);
			this->cbbListDevice->Name = L"cbbListDevice";
			this->cbbListDevice->Size = System::Drawing::Size(623, 24);
			this->cbbListDevice->TabIndex = 4;
			this->cbbListDevice->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &USBTokenManagerForm::cbbListDevice_MouseClick);
			// 
			// tbNewPINCode
			// 
			this->tbNewPINCode->Location = System::Drawing::Point(144, 117);
			this->tbNewPINCode->Name = L"tbNewPINCode";
			this->tbNewPINCode->PasswordChar = '*';
			this->tbNewPINCode->Size = System::Drawing::Size(303, 22);
			this->tbNewPINCode->TabIndex = 6;
			// 
			// lblNewPinCode
			// 
			this->lblNewPinCode->AutoSize = true;
			this->lblNewPinCode->Location = System::Drawing::Point(14, 117);
			this->lblNewPinCode->Name = L"lblNewPinCode";
			this->lblNewPinCode->Size = System::Drawing::Size(98, 17);
			this->lblNewPinCode->TabIndex = 7;
			this->lblNewPinCode->Text = L"New PIN Code";
			// 
			// grbSignature
			// 
			this->grbSignature->Controls->Add(this->lblSignatureWrite);
			this->grbSignature->Controls->Add(this->lblSignatureRead);
			this->grbSignature->Controls->Add(this->btnWriteSignature);
			this->grbSignature->Controls->Add(this->tbWriteSignature);
			this->grbSignature->Controls->Add(this->btnReadSignature);
			this->grbSignature->Controls->Add(this->tbReadSignature);
			this->grbSignature->Controls->Add(this->btnBrowseFileName);
			this->grbSignature->Controls->Add(this->tbFileName);
			this->grbSignature->Controls->Add(this->lblFileName);
			this->grbSignature->Controls->Add(this->cbbListSignature);
			this->grbSignature->Location = System::Drawing::Point(12, 187);
			this->grbSignature->Name = L"grbSignature";
			this->grbSignature->Size = System::Drawing::Size(663, 225);
			this->grbSignature->TabIndex = 14;
			this->grbSignature->TabStop = false;
			this->grbSignature->Text = L"Signatures";
			// 
			// lblSignatureWrite
			// 
			this->lblSignatureWrite->AutoSize = true;
			this->lblSignatureWrite->Location = System::Drawing::Point(14, 190);
			this->lblSignatureWrite->Name = L"lblSignatureWrite";
			this->lblSignatureWrite->Size = System::Drawing::Size(106, 17);
			this->lblSignatureWrite->TabIndex = 18;
			this->lblSignatureWrite->Text = L"Signature Write";
			// 
			// lblSignatureRead
			// 
			this->lblSignatureRead->AutoSize = true;
			this->lblSignatureRead->Location = System::Drawing::Point(14, 132);
			this->lblSignatureRead->Name = L"lblSignatureRead";
			this->lblSignatureRead->Size = System::Drawing::Size(107, 17);
			this->lblSignatureRead->TabIndex = 17;
			this->lblSignatureRead->Text = L"Signature Read";
			// 
			// btnWriteSignature
			// 
			this->btnWriteSignature->Enabled = false;
			this->btnWriteSignature->Location = System::Drawing::Point(512, 187);
			this->btnWriteSignature->Name = L"btnWriteSignature";
			this->btnWriteSignature->Size = System::Drawing::Size(128, 31);
			this->btnWriteSignature->TabIndex = 16;
			this->btnWriteSignature->Text = L"New Signature";
			this->btnWriteSignature->UseVisualStyleBackColor = true;
			this->btnWriteSignature->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnWriteSignature_Click);
			// 
			// tbWriteSignature
			// 
			this->tbWriteSignature->Location = System::Drawing::Point(144, 187);
			this->tbWriteSignature->Name = L"tbWriteSignature";
			this->tbWriteSignature->Size = System::Drawing::Size(303, 22);
			this->tbWriteSignature->TabIndex = 15;
			// 
			// btnReadSignature
			// 
			this->btnReadSignature->Enabled = false;
			this->btnReadSignature->Location = System::Drawing::Point(512, 125);
			this->btnReadSignature->Name = L"btnReadSignature";
			this->btnReadSignature->Size = System::Drawing::Size(128, 31);
			this->btnReadSignature->TabIndex = 13;
			this->btnReadSignature->Text = L"Read Signature";
			this->btnReadSignature->UseVisualStyleBackColor = true;
			this->btnReadSignature->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnReadSignature_Click);
			// 
			// tbReadSignature
			// 
			this->tbReadSignature->Location = System::Drawing::Point(144, 129);
			this->tbReadSignature->Name = L"tbReadSignature";
			this->tbReadSignature->Size = System::Drawing::Size(303, 22);
			this->tbReadSignature->TabIndex = 12;
			// 
			// btnBrowseFileName
			// 
			this->btnBrowseFileName->Location = System::Drawing::Point(512, 72);
			this->btnBrowseFileName->Name = L"btnBrowseFileName";
			this->btnBrowseFileName->Size = System::Drawing::Size(128, 32);
			this->btnBrowseFileName->TabIndex = 11;
			this->btnBrowseFileName->Text = L"Browse";
			this->btnBrowseFileName->UseVisualStyleBackColor = true;
			this->btnBrowseFileName->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnBrowseFileName_Click);
			// 
			// tbFileName
			// 
			this->tbFileName->Location = System::Drawing::Point(144, 73);
			this->tbFileName->Name = L"tbFileName";
			this->tbFileName->Size = System::Drawing::Size(303, 22);
			this->tbFileName->TabIndex = 9;
			// 
			// lblFileName
			// 
			this->lblFileName->AutoSize = true;
			this->lblFileName->Location = System::Drawing::Point(14, 78);
			this->lblFileName->Name = L"lblFileName";
			this->lblFileName->Size = System::Drawing::Size(71, 17);
			this->lblFileName->TabIndex = 7;
			this->lblFileName->Text = L"File Name";
			// 
			// cbbListSignature
			// 
			this->cbbListSignature->FormattingEnabled = true;
			this->cbbListSignature->Location = System::Drawing::Point(17, 21);
			this->cbbListSignature->Name = L"cbbListSignature";
			this->cbbListSignature->Size = System::Drawing::Size(623, 24);
			this->cbbListSignature->TabIndex = 5;
			// 
			// btnExit
			// 
			this->btnExit->Location = System::Drawing::Point(524, 418);
			this->btnExit->Name = L"btnExit";
			this->btnExit->Size = System::Drawing::Size(128, 33);
			this->btnExit->TabIndex = 15;
			this->btnExit->Text = L"Exit";
			this->btnExit->UseVisualStyleBackColor = true;
			this->btnExit->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnExit_Click);
			// 
			// USBTokenManagerForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->ClientSize = System::Drawing::Size(691, 474);
			this->Controls->Add(this->btnExit);
			this->Controls->Add(this->grbSignature);
			this->Controls->Add(this->grbDevice);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"USBTokenManagerForm";
			this->Text = L"USBTokenManager";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &USBTokenManagerForm::USBTokenManagerForm_FormClosing);
			this->grbDevice->ResumeLayout(false);
			this->grbDevice->PerformLayout();
			this->grbSignature->ResumeLayout(false);
			this->grbSignature->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void btnExit_Click(System::Object^  sender, System::EventArgs^  e)
	{
		BOOL bResult = TRUE;
		CloseDevice(&g_DeviceData);
		if (bResult == FALSE)
		{
			PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		}

		Application::Exit();
	}

	private: System::Void cbbListDevice_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		HRESULT hr;
		BOOL bResult = TRUE;
		BOOL noDevice = FALSE;

		hr = OpenDevice(&g_DeviceData, &noDevice);
		if (FAILED(hr))
		{
			if (noDevice)
			{
				DebugPrint("Device not connected or driver not installed\n");
			}
			else
			{
				DebugPrint(("Failed looking for device, HRESULT 0x%x\n"), hr);
			}
			return;
		}
		bResult = GetConfigDevice();
		if (bResult == FALSE)
		{
			PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
			return;
		}

		String^ strName = gcnew String(g_DeviceData.DevicePath);
		cbbListDevice->Items->Add(strName);
		return;
	}

	private: System::Void btnAuthenticate_Click(System::Object^  sender, System::EventArgs^  e)
	{
		btnSetPIN->Enabled = TRUE;
		btnReadSignature->Enabled = TRUE;
		btnWriteSignature->Enabled = TRUE;

		String^ strPassword = tbPINCode->Text;
		tbPINCode->ResetText();
		BOOL bResult = TRUE;
		if (strPassword->Length == 0)
		{
			MessageBox::Show(L"PIN Code Empty");
			return;
		}

		PCHAR szPassword = (char*)Marshal::StringToHGlobalAnsi(strPassword).ToPointer();
		bResult = AuthenticateDevice(szPassword);
		if (!bResult)
		{
			btnReadSignature->Enabled = FALSE;
			btnWriteSignature->Enabled = FALSE;
			btnSetPIN->Enabled = FALSE;

			MessageBox::Show(L"Authenticate PIN Failed");
			Marshal::FreeHGlobal((IntPtr)szPassword);
			return;
		}

		btnSetPIN->Enabled = TRUE;
		btnReadSignature->Enabled = TRUE;
		btnWriteSignature->Enabled = TRUE;
		Marshal::FreeHGlobal((IntPtr)szPassword);
		MessageBox::Show(L"Authenticate Successfully");
		return;
	}

	private: System::Void btnSetPIN_Click(System::Object^  sender, System::EventArgs^  e)
	{
		String^ strNewPassword = tbNewPINCode->Text;
		tbNewPINCode->ResetText();
		BOOL bResult = TRUE;
		if (strNewPassword->Length == 0)
		{
			MessageBox::Show(L"New PIN Code Empty");
			return;
		}
		PCHAR szNewPassword = (char*)Marshal::StringToHGlobalAnsi(strNewPassword).ToPointer();
		bResult = SetPasswordDevice(szNewPassword);
		if (!bResult)
		{
			MessageBox::Show(L"Set New PIN Code Failed");
			Marshal::FreeHGlobal((IntPtr)szNewPassword);
			return;
		}
		Marshal::FreeHGlobal((IntPtr)szNewPassword);
		MessageBox::Show(L"Set New PIN Code Successfully");
	}

	private: System::Void btnResetPIN_Click(System::Object^  sender, System::EventArgs^  e)
	{
		BOOL bResult = TRUE;

		bResult = SetPasswordDevice("huyhv8");
		if (!bResult)
		{
			MessageBox::Show(L"Reset PIN Code Failed");
			return;
		}
		MessageBox::Show(L"Reset PIN Code Successfully");
	}

	private: System::Void btnWriteSignature_Click(System::Object^  sender, System::EventArgs^  e)
	{
		FormInfo^ Form2 = gcnew FormInfo();
		Form2->ShowDialog();
		
// 		String^ strSignature = tbWriteSignature->Text;
// 		PCHAR szSignature = NULL;
// 		tbWriteSignature->ResetText();
// 		BOOL bResult = TRUE;
// 		if (strSignature->Length == 0)
// 		{
// 			MessageBox::Show(L"Signature Empty");
// 			return;
// 		}
// 
// 		szSignature = (PCHAR)Marshal::StringToHGlobalAnsi(strSignature).ToPointer();
// 		bResult = WriteSignature((PBYTE)szSignature, (USHORT)strlen(szSignature));
// 		if (!bResult)
// 		{
// 			MessageBox::Show(L"Write Signature To Device Failed");
// 			Marshal::FreeHGlobal((IntPtr)szSignature);
// 			return;
// 		}
// 		MessageBox::Show(L"Write Signature To Device Successfully");
// 		Marshal::FreeHGlobal((IntPtr)szSignature);
		return;
	}

	private: System::Void btnReadSignature_Click(System::Object^  sender, System::EventArgs^  e)
	{
		String^ strSignature = tbReadSignature->Text;
		USHORT usSignatureSize = 0;
		CHAR szSignature[MAX_SIGNATURE_SIZE] = { 0 };
		tbReadSignature->ResetText();
		BOOL bResult = TRUE;

		bResult = ReadSignature((PBYTE)szSignature, &usSignatureSize);
		if (bResult == FALSE || usSignatureSize == 0)
		{
			MessageBox::Show(L"Read Signature From Device Failed");
			return;
		}

		String^ strName = gcnew String(szSignature);
		tbReadSignature->Text = strName;
		return;
	}

	private: System::Void btnBrowseFileName_Click(System::Object^  sender, System::EventArgs^  e)
	{
		tbFileName->ResetText();
		OpenFileDialog^ openFileDialogSign = gcnew OpenFileDialog;

		openFileDialogSign->InitialDirectory = "c:\\";
		openFileDialogSign->Filter = "exe files (*.exe)|*.exe|All files (*.*)|*.*";
		openFileDialogSign->FilterIndex = 2;
		openFileDialogSign->RestoreDirectory = true;

		if (openFileDialogSign->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			String^ FileName = openFileDialogSign->FileName;
			tbFileName->Text = FileName;
		}
		return;
	}

	private: System::Void USBTokenManagerForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
	{
		BOOL bResult = TRUE;
		CloseDevice(&g_DeviceData);
		if (bResult == FALSE)
		{
			PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		}
	}
	};
}