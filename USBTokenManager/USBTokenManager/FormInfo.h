﻿#pragma once

#include "Common.h"
#include "device.h"
#include <vcclr.h>

using namespace System;
using namespace System::Security::Cryptography;
using namespace System::Security::Cryptography::X509Certificates;
using namespace std;
using namespace System::Runtime::InteropServices;
using namespace System::Text;
using namespace System::IO;
using namespace System::Diagnostics;
using namespace Org::BouncyCastle::OpenSsl;
using namespace Org::BouncyCastle::Crypto;
using namespace Org::BouncyCastle::Security;
using namespace Org::BouncyCastle::Crypto::Parameters;

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
	private: System::Windows::Forms::TextBox^  tbSubjectName;


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
			this->tbSubjectName = (gcnew System::Windows::Forms::TextBox());
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
			this->grbDevice->Controls->Add(this->tbSubjectName);
			this->grbDevice->Controls->Add(this->lblPassword);
			this->grbDevice->Location = System::Drawing::Point(12, 12);
			this->grbDevice->Name = L"grbDevice";
			this->grbDevice->Size = System::Drawing::Size(405, 158);
			this->grbDevice->TabIndex = 14;
			this->grbDevice->TabStop = false;
			this->grbDevice->Text = L"Account";
			// 
			// btnExit
			// 
			this->btnExit->Location = System::Drawing::Point(272, 106);
			this->btnExit->Name = L"btnExit";
			this->btnExit->Size = System::Drawing::Size(112, 32);
			this->btnExit->TabIndex = 12;
			this->btnExit->Text = L"Exit";
			this->btnExit->UseVisualStyleBackColor = true;
			this->btnExit->Click += gcnew System::EventHandler(this, &FormInfo::btnExit_Click);
			// 
			// btnCreateSignature
			// 
			this->btnCreateSignature->Location = System::Drawing::Point(99, 106);
			this->btnCreateSignature->Name = L"btnCreateSignature";
			this->btnCreateSignature->Size = System::Drawing::Size(138, 32);
			this->btnCreateSignature->TabIndex = 11;
			this->btnCreateSignature->Text = L"Generate Key Pair";
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
			// tbSubjectName
			// 
			this->tbSubjectName->Location = System::Drawing::Point(128, 62);
			this->tbSubjectName->Name = L"tbSubjectName";
			this->tbSubjectName->Size = System::Drawing::Size(221, 22);
			this->tbSubjectName->TabIndex = 6;
			// 
			// lblPassword
			// 
			this->lblPassword->AutoSize = true;
			this->lblPassword->Location = System::Drawing::Point(6, 67);
			this->lblPassword->Name = L"lblPassword";
			this->lblPassword->Size = System::Drawing::Size(96, 17);
			this->lblPassword->TabIndex = 7;
			this->lblPassword->Text = L"Subject Name";
			// 
			// FormInfo
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(433, 180);
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

		if (strUserName->Length == 0)
		{
			MessageBox::Show(L"UserName Empty");
			return;
		}

		String^ strSubjectName = tbSubjectName->Text;
		if (strSubjectName->Length == 0)
		{
			MessageBox::Show(L"Password Empty");
			return;
		}

		RSACryptoServiceProvider^ RSA = gcnew RSACryptoServiceProvider(2048);
		String^ RSAPubLicKey = gcnew String(RSA->ToXmlString(FALSE));

		String^ RSAPrivateKey = gcnew String(RSA->ToXmlString(TRUE));

		DebugPrint("Private Key: %s", RSAPrivateKey);
		DebugPrint("Public Key: %s", RSAPubLicKey);

		System::IO::File::WriteAllText(tbUserName->Text + ".pub", RSAPubLicKey);
		System::IO::File::AppendAllText(tbUserName->Text + ".pub", "\r\n");
		System::IO::File::AppendAllText(tbUserName->Text + ".pub", tbSubjectName->Text);

		tbUserName->ResetText();
		tbSubjectName->ResetText();

		PCHAR szXmlPrivateKey = NULL;
		szXmlPrivateKey = (PCHAR)Marshal::StringToHGlobalAnsi(RSAPrivateKey).ToPointer();

		DebugPrint("Size of Private key: %d", strlen(szXmlPrivateKey));
		bResult = WriteSignature((PBYTE)szXmlPrivateKey, (USHORT)strlen(szXmlPrivateKey));
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
