#pragma once

#include "Common.h"
#include "device.h"
#include <vcclr.h>

using namespace System;
using namespace System::Security::Cryptography;
using namespace std;
using namespace System::Runtime::InteropServices;
using namespace System::Text;
using namespace System::IO;
using namespace System::Diagnostics;

namespace USBTokenManager {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FormInfo
	/// </summary>
	public ref class FormInfo : public System::Windows::Forms::Form
	{
	public:
		FormInfo(void)
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
		~FormInfo()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::GroupBox^  grbDevice;
	private: System::Windows::Forms::Button^  btnCreateSignature;
	protected:



	private: System::Windows::Forms::Label^  lblUserName;
	private: System::Windows::Forms::TextBox^  tbUserName;
	private: System::Windows::Forms::TextBox^  tbPassword;

	private: System::Windows::Forms::TextBox^  tbNewPINCode;
	private: System::Windows::Forms::Label^  lblPassword;
	private: System::Windows::Forms::Button^  btnExit;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormInfo::typeid));
			this->grbDevice = (gcnew System::Windows::Forms::GroupBox());
			this->btnExit = (gcnew System::Windows::Forms::Button());
			this->btnCreateSignature = (gcnew System::Windows::Forms::Button());
			this->lblUserName = (gcnew System::Windows::Forms::Label());
			this->tbUserName = (gcnew System::Windows::Forms::TextBox());
			this->tbPassword = (gcnew System::Windows::Forms::TextBox());
			this->lblPassword = (gcnew System::Windows::Forms::Label());
			this->grbDevice->SuspendLayout();
			this->SuspendLayout();
			// 
			// grbDevice
			// 
			this->grbDevice->Controls->Add(this->btnExit);
			this->grbDevice->Controls->Add(this->btnCreateSignature);
			this->grbDevice->Controls->Add(this->lblUserName);
			this->grbDevice->Controls->Add(this->tbUserName);
			this->grbDevice->Controls->Add(this->tbPassword);
			this->grbDevice->Controls->Add(this->lblPassword);
			this->grbDevice->Location = System::Drawing::Point(12, 12);
			this->grbDevice->Name = L"grbDevice";
			this->grbDevice->Size = System::Drawing::Size(387, 158);
			this->grbDevice->TabIndex = 14;
			this->grbDevice->TabStop = false;
			this->grbDevice->Text = L"Account";
			// 
			// btnExit
			// 
			this->btnExit->Location = System::Drawing::Point(270, 106);
			this->btnExit->Name = L"btnExit";
			this->btnExit->Size = System::Drawing::Size(107, 32);
			this->btnExit->TabIndex = 12;
			this->btnExit->Text = L"Exit";
			this->btnExit->UseVisualStyleBackColor = true;
			this->btnExit->Click += gcnew System::EventHandler(this, &FormInfo::btnExit_Click);
			// 
			// btnCreateSignature
			// 
			this->btnCreateSignature->Location = System::Drawing::Point(98, 106);
			this->btnCreateSignature->Name = L"btnCreateSignature";
			this->btnCreateSignature->Size = System::Drawing::Size(125, 32);
			this->btnCreateSignature->TabIndex = 11;
			this->btnCreateSignature->Text = L"Create Signature";
			this->btnCreateSignature->UseVisualStyleBackColor = true;
			this->btnCreateSignature->Click += gcnew System::EventHandler(this, &FormInfo::btnCreateSignature_Click);
			// 
			// lblUserName
			// 
			this->lblUserName->AutoSize = true;
			this->lblUserName->Location = System::Drawing::Point(6, 24);
			this->lblUserName->Name = L"lblUserName";
			this->lblUserName->Size = System::Drawing::Size(79, 17);
			this->lblUserName->TabIndex = 6;
			this->lblUserName->Text = L"User Name";
			// 
			// tbUserName
			// 
			this->tbUserName->Location = System::Drawing::Point(128, 21);
			this->tbUserName->Name = L"tbUserName";
			this->tbUserName->Size = System::Drawing::Size(221, 22);
			this->tbUserName->TabIndex = 8;
			this->tbUserName->TextChanged += gcnew System::EventHandler(this, &FormInfo::tbUserName_TextChanged);
			// 
			// tbPassword
			// 
			this->tbPassword->Location = System::Drawing::Point(128, 62);
			this->tbPassword->Name = L"tbPassword";
			this->tbPassword->PasswordChar = '*';
			this->tbPassword->Size = System::Drawing::Size(221, 22);
			this->tbPassword->TabIndex = 6;
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
			this->lblPassword->Location = System::Drawing::Point(6, 67);
			this->lblPassword->Name = L"lblPassword";
			this->lblPassword->Size = System::Drawing::Size(69, 17);
			this->lblPassword->TabIndex = 7;
			this->lblPassword->Text = L"Password";
			// 
			// FormInfo
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(407, 180);
			this->Controls->Add(this->grbDevice);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"FormInfo";
			this->Text = L"USBTokenManager";
			this->grbDevice->ResumeLayout(false);
			this->grbDevice->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void tbUserName_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void btnExit_Click(System::Object^  sender, System::EventArgs^  e) {
		this->Close();
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

	private: System::Void btnCreateSignature_Click(System::Object^  sender, System::EventArgs^  e)
	{
		BOOL bResult = TRUE;

		String^ strUserName = tbUserName->Text;
		tbUserName->ResetText();
		if (strUserName->Length == 0)
		{
			MessageBox::Show(L"UserName Empty");
			return;
		}

		String^ strPassword = tbPassword->Text;
		tbPassword->ResetText();
		if (strPassword->Length == 0)
		{
			MessageBox::Show(L"Password Empty");
			return;
		}

		RSACryptoServiceProvider^ RSA = gcnew RSACryptoServiceProvider(2048);
		String^ RSAPubLicKey = gcnew String(RSA->ToXmlString(FALSE));
		String^ RSAPrivateKey = gcnew String(RSA->ToXmlString(TRUE));

		// 		DebugPrint("Private Key: %s", RSAPrivateKey);
		// 		DebugPrint("Public Key: %s", RSAPubLicKey);

		PCHAR szXmlPrivateKey = NULL;
		szXmlPrivateKey = (PCHAR)Marshal::StringToHGlobalAnsi(RSAPrivateKey).ToPointer();
		/*		DebugPrint("szSignature: %s", szXmlPrivateKey);*/

		String^ fileName = gcnew String("D:\\AccessTokenGitLab.txt");
		FileStream^ stream = System::IO::File::OpenRead(fileName);

		SHA256Managed^ sha = gcnew SHA256Managed();

		cli::array<unsigned char>^ hash = sha->ComputeHash(stream);

		DebugPrint("hash file: %s", Convert::ToBase64String(hash));

		RSA->FromXmlString(RSAPubLicKey);
		cli::array<unsigned char>^ Sign = RSA->Encrypt(hash, TRUE);

		DebugPrint("Encrypt Message: %s", Convert::ToBase64String(Sign));

		//		String^ strCert = Encoding::Unicode->GetString(Sign);
		System::IO::File::WriteAllText("D:\\AccessTokenGitLab.sign", Convert::ToBase64String(Sign));


		String^ ReadFromFile;
		ReadFromFile = System::IO::File::ReadAllText("D:\\AccessTokenGitLab.sign");
		RSA->FromXmlString(RSAPrivateKey);
		cli::array<unsigned char>^ DecryptMess = RSA->Decrypt(Convert::FromBase64String(ReadFromFile), TRUE);
		DebugPrint("Decrypt Message: %s", Convert::ToBase64String(DecryptMess));
		// 		String^ fileName = gcnew String("D:\\AccessTokenGitLab.txt");
		// 		FileStream^ stream = System::IO::File::OpenRead(fileName);

		/*		SHA256Managed^ sha = gcnew SHA256Managed();*/
		cli::array<unsigned char>^ hash1 = sha->ComputeHash(stream);
		BOOL bIsSignatureValid = TRUE;

		Boolean bIsEqual = IsEqualValue(DecryptMess, hash);
		if (bIsEqual == TRUE)
		{
			MessageBox::Show(L"Signature valid");
		}
		else
		{
			MessageBox::Show(L"Signature invalid");
		}


		//Crash khi gui c?c nhi?u nhu này, có th? do size du?i firmware nh? hon
		DebugPrint("Size of Private key: %d", strlen(szXmlPrivateKey));
		bResult = WriteSignature((PBYTE)szXmlPrivateKey, strlen(szXmlPrivateKey));
		if (!bResult)
		{
			MessageBox::Show(L"Write Signature To Device Failed");
			Marshal::FreeHGlobal((IntPtr)szXmlPrivateKey);
			return;
		}

		MessageBox::Show(L"Write Signature To Device Successfully");
		Marshal::FreeHGlobal((IntPtr)szXmlPrivateKey);

		return;

	}
	};
}
