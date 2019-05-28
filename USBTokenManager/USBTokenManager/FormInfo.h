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
		RSAParameters RSAKeyInfo = RSA->ExportParameters(true);

		DebugPrint("Private Key: %s", RSAKeyInfo.D);
		DebugPrint("Public Key: %s", RSAKeyInfo.Modulus);

//		String^ strModulus = gcnew String((String^)RSAKeyInfo.Modulus);
		String^ strModulus = System::Convert::ToBase64String(RSAKeyInfo.Modulus);

/*		RSAKeyInfo.Modulus = System::Convert::FromBase64String(strModulus);*/
		PCHAR szSignature = NULL;

		szSignature = (PCHAR)Marshal::StringToHGlobalAnsi(strModulus).ToPointer();
		DebugPrint("szSignature: %s", szSignature);

		bResult = WriteSignature((PBYTE)szSignature, 256);
		if (!bResult)
		{
			MessageBox::Show(L"Write Signature To Device Failed");
			Marshal::FreeHGlobal((IntPtr)szSignature);
			return;
		}
		MessageBox::Show(L"Write Signature To Device Successfully");
		Marshal::FreeHGlobal((IntPtr)szSignature);

		return;

	}
	};
}
