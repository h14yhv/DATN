#pragma once
#include "device.h"
#include "Common.h"
#include "string.h"
#include<vcclr.h>

using namespace std;
using namespace System::Runtime::InteropServices;

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

	private: System::Windows::Forms::Button^  btnResetPIN;

	private: System::Windows::Forms::Button^  btnSetPIN;

	private: System::Windows::Forms::Label^  lblPINCode;
	private: System::Windows::Forms::TextBox^  tbPINCode;


	private: System::Windows::Forms::ComboBox^  cbbListDevice;
	private: System::Windows::Forms::TextBox^  tbNewPINCode;


	private: System::Windows::Forms::Label^  lblNewPinCode;
	private: System::Windows::Forms::GroupBox^  grbSignature;
	private: System::Windows::Forms::Button^  btnWriteSignature;



	private: System::Windows::Forms::Button^  btnReadSignature;

	private: System::Windows::Forms::TextBox^  tbSignature;

	private: System::Windows::Forms::Button^  btnBrowseFileName;

	private: System::Windows::Forms::TextBox^  tbFileName;

	private: System::Windows::Forms::Label^  lblFileName;

	private: System::Windows::Forms::ComboBox^  cbbListSignature;
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
			this->grbDevice = (gcnew System::Windows::Forms::GroupBox());
			this->btnAuthenticate = (gcnew System::Windows::Forms::Button());
			this->btnResetPIN = (gcnew System::Windows::Forms::Button());
			this->btnSetPIN = (gcnew System::Windows::Forms::Button());
			this->lblPINCode = (gcnew System::Windows::Forms::Label());
			this->tbPINCode = (gcnew System::Windows::Forms::TextBox());
			this->cbbListDevice = (gcnew System::Windows::Forms::ComboBox());
			this->tbNewPINCode = (gcnew System::Windows::Forms::TextBox());
			this->lblNewPinCode = (gcnew System::Windows::Forms::Label());
			this->grbSignature = (gcnew System::Windows::Forms::GroupBox());
			this->btnWriteSignature = (gcnew System::Windows::Forms::Button());
			this->btnReadSignature = (gcnew System::Windows::Forms::Button());
			this->tbSignature = (gcnew System::Windows::Forms::TextBox());
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
			this->grbDevice->Controls->Add(this->btnResetPIN);
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
			this->btnAuthenticate->Location = System::Drawing::Point(331, 59);
			this->btnAuthenticate->Name = L"btnAuthenticate";
			this->btnAuthenticate->Size = System::Drawing::Size(116, 22);
			this->btnAuthenticate->TabIndex = 11;
			this->btnAuthenticate->Text = L"Authenticate";
			this->btnAuthenticate->UseVisualStyleBackColor = true;
			this->btnAuthenticate->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnAuthenticate_Click);
			// 
			// btnResetPIN
			// 
			this->btnResetPIN->Location = System::Drawing::Point(512, 58);
			this->btnResetPIN->Name = L"btnResetPIN";
			this->btnResetPIN->Size = System::Drawing::Size(116, 23);
			this->btnResetPIN->TabIndex = 10;
			this->btnResetPIN->Text = L"Reset PIN";
			this->btnResetPIN->UseVisualStyleBackColor = true;
			// 
			// btnSetPIN
			// 
			this->btnSetPIN->Location = System::Drawing::Point(331, 117);
			this->btnSetPIN->Name = L"btnSetPIN";
			this->btnSetPIN->Size = System::Drawing::Size(116, 23);
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
			this->tbPINCode->Location = System::Drawing::Point(128, 59);
			this->tbPINCode->Name = L"tbPINCode";
			this->tbPINCode->PasswordChar = '*';
			this->tbPINCode->Size = System::Drawing::Size(118, 22);
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
			this->tbNewPINCode->Location = System::Drawing::Point(128, 117);
			this->tbNewPINCode->Name = L"tbNewPINCode";
			this->tbNewPINCode->PasswordChar = '*';
			this->tbNewPINCode->Size = System::Drawing::Size(118, 22);
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
			this->grbSignature->Controls->Add(this->btnWriteSignature);
			this->grbSignature->Controls->Add(this->btnReadSignature);
			this->grbSignature->Controls->Add(this->tbSignature);
			this->grbSignature->Controls->Add(this->btnBrowseFileName);
			this->grbSignature->Controls->Add(this->tbFileName);
			this->grbSignature->Controls->Add(this->lblFileName);
			this->grbSignature->Controls->Add(this->cbbListSignature);
			this->grbSignature->Location = System::Drawing::Point(12, 187);
			this->grbSignature->Name = L"grbSignature";
			this->grbSignature->Size = System::Drawing::Size(663, 170);
			this->grbSignature->TabIndex = 14;
			this->grbSignature->TabStop = false;
			this->grbSignature->Text = L"Signatures";
			// 
			// btnWriteSignature
			// 
			this->btnWriteSignature->Location = System::Drawing::Point(512, 122);
			this->btnWriteSignature->Name = L"btnWriteSignature";
			this->btnWriteSignature->Size = System::Drawing::Size(128, 23);
			this->btnWriteSignature->TabIndex = 14;
			this->btnWriteSignature->Text = L"Write Signature";
			this->btnWriteSignature->UseVisualStyleBackColor = true;
			// 
			// btnReadSignature
			// 
			this->btnReadSignature->Location = System::Drawing::Point(357, 121);
			this->btnReadSignature->Name = L"btnReadSignature";
			this->btnReadSignature->Size = System::Drawing::Size(116, 23);
			this->btnReadSignature->TabIndex = 13;
			this->btnReadSignature->Text = L"Read Signature";
			this->btnReadSignature->UseVisualStyleBackColor = true;
			// 
			// tbSignature
			// 
			this->tbSignature->Location = System::Drawing::Point(17, 122);
			this->tbSignature->Name = L"tbSignature";
			this->tbSignature->Size = System::Drawing::Size(319, 22);
			this->tbSignature->TabIndex = 12;
			// 
			// btnBrowseFileName
			// 
			this->btnBrowseFileName->Location = System::Drawing::Point(512, 78);
			this->btnBrowseFileName->Name = L"btnBrowseFileName";
			this->btnBrowseFileName->Size = System::Drawing::Size(128, 23);
			this->btnBrowseFileName->TabIndex = 11;
			this->btnBrowseFileName->Text = L"Browse";
			this->btnBrowseFileName->UseVisualStyleBackColor = true;
			// 
			// tbFileName
			// 
			this->tbFileName->Location = System::Drawing::Point(128, 73);
			this->tbFileName->Name = L"tbFileName";
			this->tbFileName->Size = System::Drawing::Size(319, 22);
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
			this->btnExit->Location = System::Drawing::Point(524, 363);
			this->btnExit->Name = L"btnExit";
			this->btnExit->Size = System::Drawing::Size(128, 23);
			this->btnExit->TabIndex = 15;
			this->btnExit->Text = L"Exit";
			this->btnExit->UseVisualStyleBackColor = true;
			this->btnExit->Click += gcnew System::EventHandler(this, &USBTokenManagerForm::btnExit_Click);
			// 
			// USBTokenManagerForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(684, 395);
			this->Controls->Add(this->btnExit);
			this->Controls->Add(this->grbSignature);
			this->Controls->Add(this->grbDevice);
			this->Name = L"USBTokenManagerForm";
			this->Text = L"USBTokenManager";
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
		BOOL bResult = TRUE;
		String^ strName = gcnew String(g_DeviceData.DevicePath);

		bResult = GetConfigDevice();
		if (bResult == FALSE)
		{
			PrintError("Function %s failed at %d in %s", __FUNCTION__, __LINE__, __FILE__);
		}

		cbbListDevice->Items->Add(strName);
	}
	private: System::Void btnAuthenticate_Click(System::Object^  sender, System::EventArgs^  e)
	{
		String^ strPassword = tbPINCode->Text;
		tbPINCode->ResetText();
		BOOL bResult = TRUE; 
		if (strPassword->Length == 0)
		{
			MessageBox::Show(L"PIN Code Empty");
			return;
		}
		bResult = AuthenticateDevice((char*)Marshal::StringToHGlobalAnsi(strPassword).ToPointer());
		if (!bResult)
		{
			MessageBox::Show(L"Authenticate PIN Failed");
			return;
		}
		MessageBox::Show(L"Authenticate Successfully");
		return;
	}

	private: System::Void btnSetPIN_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		String^ strNewPassword = tbNewPINCode->Text;
		tbNewPINCode->Clear();
		BOOL bResult = TRUE;
		if (strNewPassword->Length == 0)
		{
			MessageBox::Show(L"New PIN Code Empty");
			return;
		}
		bResult = SetPasswordDevice((char*)Marshal::StringToHGlobalAnsi(strNewPassword).ToPointer());
		if (!bResult)
		{
			MessageBox::Show(L"Set New PIN Code Failed");
			return;
		}
		MessageBox::Show(L"Set New PIN Code Successfully");
	}
};
}