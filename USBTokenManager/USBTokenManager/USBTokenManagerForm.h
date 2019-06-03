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


	private: System::Windows::Forms::Button^  btnBrowseFileName;

	private: System::Windows::Forms::TextBox^  tbFileName;

	private: System::Windows::Forms::Label^  lblFileName;


	private: System::Windows::Forms::Button^  btnExit;




	private: System::Windows::Forms::Button^  btnWriteSignature;

	private: System::Windows::Forms::Label^  lbPublicFilePath;


	private: System::Windows::Forms::TextBox^  tbPublicPath;
	private: System::Windows::Forms::Button^  btbPublicKeyPath;
	private: System::Windows::Forms::Button^  btnVerifySignature;



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
			this->btbPublicKeyPath = (gcnew System::Windows::Forms::Button());
			this->btnVerifySignature = (gcnew System::Windows::Forms::Button());
			this->tbPublicPath = (gcnew System::Windows::Forms::TextBox());
			this->btnWriteSignature = (gcnew System::Windows::Forms::Button());
			this->lbPublicFilePath = (gcnew System::Windows::Forms::Label());
			this->btnReadSignature = (gcnew System::Windows::Forms::Button());
			this->btnBrowseFileName = (gcnew System::Windows::Forms::Button());
			this->tbFileName = (gcnew System::Windows::Forms::TextBox());
			this->lblFileName = (gcnew System::Windows::Forms::Label());
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
			this->grbDevice->Size = System::Drawing::Size(663, 160);
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
			this->grbSignature->Controls->Add(this->btbPublicKeyPath);
			this->grbSignature->Controls->Add(this->btnVerifySignature);
			this->grbSignature->Controls->Add(this->tbPublicPath);
			this->grbSignature->Controls->Add(this->btnWriteSignature);
			this->grbSignature->Controls->Add(this->lbPublicFilePath);
			this->grbSignature->Controls->Add(this->btnReadSignature);
			this->grbSignature->Controls->Add(this->btnBrowseFileName);
			this->grbSignature->Controls->Add(this->tbFileName);
			this->grbSignature->Controls->Add(this->lblFileName);
			this->grbSignature->Location = System::Drawing::Point(12, 187);
			this->grbSignature->Name = L"grbSignature";
			this->grbSignature->Size = System::Drawing::Size(663, 175);
			this->grbSignature->TabIndex = 14;
			this->grbSignature->TabStop = false;
			this->grbSignature->Text = L"Signatures";
			// 
			// btbPublicKeyPath
			// 
			this->btbPublicKeyPath->Location = System::Drawing::Point(512, 79);
			this->btbPublicKeyPath->Name = L"btbPublicKeyPath";
			this->btbPublicKeyPath->Size = System::Drawing::Size(128, 32);
			this->btbPublicKeyPath->TabIndex = 22;
			this->btbPublicKeyPath->Text = L"Browse";
			this->btbPublicKeyPath->UseVisualStyleBackColor = true;
			this->btbPublicKeyPath->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnPublicKeyPath_Click);
			// 
			// btnVerifySignature
			// 
			this->btnVerifySignature->Location = System::Drawing::Point(352, 137);
			this->btnVerifySignature->Name = L"btnVerifySignature";
			this->btnVerifySignature->Size = System::Drawing::Size(128, 32);
			this->btnVerifySignature->TabIndex = 21;
			this->btnVerifySignature->Text = L"Verify Signature";
			this->btnVerifySignature->UseVisualStyleBackColor = true;
			this->btnVerifySignature->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnVerifySignature_Click);
			// 
			// tbPublicPath
			// 
			this->tbPublicPath->Location = System::Drawing::Point(144, 84);
			this->tbPublicPath->Name = L"tbPublicPath";
			this->tbPublicPath->Size = System::Drawing::Size(303, 22);
			this->tbPublicPath->TabIndex = 20;
			// 
			// btnWriteSignature
			// 
			this->btnWriteSignature->Enabled = false;
			this->btnWriteSignature->Location = System::Drawing::Point(195, 138);
			this->btnWriteSignature->Name = L"btnWriteSignature";
			this->btnWriteSignature->Size = System::Drawing::Size(128, 31);
			this->btnWriteSignature->TabIndex = 16;
			this->btnWriteSignature->Text = L"Generate Key";
			this->btnWriteSignature->UseVisualStyleBackColor = true;
			this->btnWriteSignature->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnGenKey_Click);
			// 
			// lbPublicFilePath
			// 
			this->lbPublicFilePath->AutoSize = true;
			this->lbPublicFilePath->Location = System::Drawing::Point(14, 87);
			this->lbPublicFilePath->Name = L"lbPublicFilePath";
			this->lbPublicFilePath->Size = System::Drawing::Size(133, 17);
			this->lbPublicFilePath->TabIndex = 19;
			this->lbPublicFilePath->Text = L"Public Key File Path";
			// 
			// btnReadSignature
			// 
			this->btnReadSignature->Enabled = false;
			this->btnReadSignature->Location = System::Drawing::Point(512, 135);
			this->btnReadSignature->Name = L"btnReadSignature";
			this->btnReadSignature->Size = System::Drawing::Size(128, 31);
			this->btnReadSignature->TabIndex = 13;
			this->btnReadSignature->Text = L"Sign File";
			this->btnReadSignature->UseVisualStyleBackColor = true;
			this->btnReadSignature->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnSignFile_Click);
			// 
			// btnBrowseFileName
			// 
			this->btnBrowseFileName->Location = System::Drawing::Point(512, 26);
			this->btnBrowseFileName->Name = L"btnBrowseFileName";
			this->btnBrowseFileName->Size = System::Drawing::Size(128, 32);
			this->btnBrowseFileName->TabIndex = 11;
			this->btnBrowseFileName->Text = L"Browse";
			this->btnBrowseFileName->UseVisualStyleBackColor = true;
			this->btnBrowseFileName->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnBrowseFileName_Click);
			// 
			// tbFileName
			// 
			this->tbFileName->Location = System::Drawing::Point(144, 31);
			this->tbFileName->Name = L"tbFileName";
			this->tbFileName->Size = System::Drawing::Size(303, 22);
			this->tbFileName->TabIndex = 9;
			// 
			// lblFileName
			// 
			this->lblFileName->AutoSize = true;
			this->lblFileName->Location = System::Drawing::Point(14, 31);
			this->lblFileName->Name = L"lblFileName";
			this->lblFileName->Size = System::Drawing::Size(63, 17);
			this->lblFileName->TabIndex = 7;
			this->lblFileName->Text = L"File Path";
			// 
			// btnExit
			// 
			this->btnExit->Location = System::Drawing::Point(524, 368);
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
			this->ClientSize = System::Drawing::Size(691, 411);
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

	private: System::Void btnGenKey_Click(System::Object^  sender, System::EventArgs^  e)
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

	private: System::Void btnSignFile_Click(System::Object^  sender, System::EventArgs^  e)
	{
		USHORT usPrivateKeySize = 0;
		CHAR szPrivateKey[MAX_SIGNATURE_SIZE] = { 0 };
		BOOL bResult = TRUE;

		bResult = ReadSignature((PBYTE)szPrivateKey, &usPrivateKeySize);
		if (bResult == FALSE || usPrivateKeySize == 0)
		{
			MessageBox::Show(L"Read Signature From Device Failed");
			return;
		}

		FileStream^ stream;
		try
		{
			stream = System::IO::File::OpenRead(tbFileName->Text);
		}
		catch (const std::exception&)
		{
			PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
			return;
		}

		//Hash file with SHA256
		SHA256Managed^ sha = gcnew SHA256Managed();
		cli::array<unsigned char>^ strHashFile;
		try
		{
			strHashFile = sha->ComputeHash(stream);
		}
		catch (const std::exception&)
		{
			PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
			return;
		}

		DebugPrint("File Hashed: %s", Convert::ToBase64String(strHashFile));

		//Lấy chữ ký từ thiết bị và tạo file lưu chữ ký
		RSACryptoServiceProvider^ RSA = gcnew RSACryptoServiceProvider(2048);
		String^ RSAPrivateKey = gcnew String(szPrivateKey);
		RSA->FromXmlString(RSAPrivateKey);

		cli::array<unsigned char>^ SignForFile;
		try
		{
			SignForFile = RSA->Encrypt(strHashFile, TRUE);
		}
		catch (const std::exception&)
		{
			PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
			return;
		}

		DebugPrint("Encrypt Message: %s", Convert::ToBase64String(SignForFile));

		//		String^ strCert = Encoding::Unicode->GetString(Sign);
		System::IO::File::WriteAllText(tbFileName->Text->Substring(0, tbFileName->Text->LastIndexOf(".")) + ".sign", Convert::ToBase64String(SignForFile));

		String^ strName = gcnew String(szPrivateKey);
		return;
	}

	private: System::Void btnBrowseFileName_Click(System::Object^  sender, System::EventArgs^  e)
	{
		tbFileName->ResetText();
		OpenFileDialog^ openFileDialogSign = gcnew OpenFileDialog;

		openFileDialogSign->InitialDirectory = Application::StartupPath;
		openFileDialogSign->Filter = "exe files (*.exe)|*.exe|All files (*.*)|*.*";
		openFileDialogSign->FilterIndex = 2;
		openFileDialogSign->RestoreDirectory = true;

		if (openFileDialogSign->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			if (openFileDialogSign->FileName->Length == 0)
			{
				return;
			}
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

	private: System::Void btnPublicKeyPath_Click(System::Object^  sender, System::EventArgs^  e)
	{
		tbPublicPath->ResetText();
		OpenFileDialog^ openFileDialogSign = gcnew OpenFileDialog;

		openFileDialogSign->InitialDirectory = Application::StartupPath;
		openFileDialogSign->Filter = "pub files (*.pub)|*.pub|All files (*.*)|*.*";
		openFileDialogSign->FilterIndex = 2;
		openFileDialogSign->RestoreDirectory = true;

		if (openFileDialogSign->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			if (openFileDialogSign->FileName->Length == 0)
			{
				return;
			}
			String^ FileName = openFileDialogSign->FileName;
			tbPublicPath->Text = FileName;
		}
	}

	private: System::Void btnVerifySignature_Click(System::Object^  sender, System::EventArgs^  e)
	{
		//Làm nốt hàm Verify
		RSACryptoServiceProvider^ RSA = gcnew RSACryptoServiceProvider(2048);
		String^ PublicKeyData = System::IO::File::ReadAllText(tbPublicPath->Text);
		try
		{
			RSA->FromXmlString(PublicKeyData);
		}
		catch (const std::exception&)
		{
			PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
			return;
		}

		String^ SignFromFile = System::IO::File::ReadAllText(tbFileName->Text->Substring(0, tbFileName->Text->LastIndexOf(".")) + ".sign");
		cli::array<unsigned char>^ DecryptMess;
		try
		{
			//Chỉ có public key, đang decrypt lỗi
			DecryptMess = RSA->Decrypt(Convert::FromBase64String(SignFromFile), FALSE);
		}
		catch (const std::exception&)
		{
			PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
			return;
		}

		DebugPrint("Decrypt Message: %s", Convert::ToBase64String(DecryptMess));

		//Sign file hiện tại
		FileStream^ StreamFileVerify;
		try
		{
			StreamFileVerify = System::IO::File::OpenRead(tbFileName->Text);
		}
		catch (const std::exception&)
		{
			PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
			return;
		}
		SHA256Managed^ sha = gcnew SHA256Managed();
		cli::array<unsigned char>^ HashFile = sha->ComputeHash(StreamFileVerify);
		BOOL bIsSignatureValid = TRUE;

		Boolean bIsEqual = IsEqualValue(DecryptMess, HashFile);
		if (bIsEqual == TRUE)
		{
			MessageBox::Show(L"Signature valid");
		}
		else
		{
			MessageBox::Show(L"Signature invalid");
		}
	}

	public: System::Boolean IsEqualValue(cli::array<unsigned char>^ arr1, cli::array<unsigned char>^ arr2)
	{
		if (arr1->Length != arr2->Length)
			return FALSE;

		for (int i = 0; i < arr1->Length; i++)
		{
			if (arr1[i] != arr2[i])
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	};
}