//==============================================================================
/*
    \author    Your Name
*/
//==============================================================================

#pragma once

//------------------------------------------------------------------------------
#include "CApplication.h"
#include "CViewport.h"
//------------------------------------------------------------------------------

namespace MyApplication {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    
    /// <summary>
    /// Summary for MainForm
    /// </summary>
    public ref class MainForm : public System::Windows::Forms::Form
    {
    public:
        MainForm(void)
        {
            InitializeComponent();

            //------------------------------------------------------------------
            // CHAI3D - INITIALIZATION
            //------------------------------------------------------------------

            // intialize variables
            moveCamera = false;

            // create application
            application= new cApplication();
            
            // initilize display size
            application->resizeGL(this->panelView->Size.Width, this->panelView->Size.Height);

            // create OpenGL viewport 
            viewport = new cViewport((HWND)this->panelView->Handle.ToPointer());

            // initialize GLEW library (OpenGL)
            viewport->preRender();
            glewInit();
            viewport->postRender();

            // start haptics thread
            application->start();			
        }

    protected:
        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        ~MainForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private: System::Windows::Forms::Panel^  panelView;
    private: System::Windows::Forms::GroupBox^  groupBox;
    private: System::Windows::Forms::MenuStrip^  menuStrip;
    private: System::Windows::Forms::ToolStripMenuItem^  menuToolStripMenuItem;
    private: System::ComponentModel::IContainer^  components;

    private:
        /// <summary>
        /// Required designer variable.
        /// </summary>

        //------------------------------------------------------------------
        // CHAI3D - OBJECTS
        //------------------------------------------------------------------

        // application
        cApplication* application;

        // mouse information
        int mouseX;
        int mouseY;
        bool moveCamera;

        // display viewport
        cViewport* viewport;
    private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
    private: System::Windows::Forms::Label^  label1;

    private: System::Windows::Forms::StatusStrip^  statusStrip;
    private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel;
















	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::RadioButton^  radioButton3;




	private: System::Windows::Forms::TrackBar^  trackBar1;

	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TrackBar^  trackBar2;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button1;
private: System::Windows::Forms::Label^  label2;

private: System::Windows::Forms::Label^  label6;


private: System::Windows::Forms::Button^  button5;
private: System::Windows::Forms::Button^  button4;
private: System::Windows::Forms::ToolStripMenuItem^  viewToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  resetViewToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  resetWorldToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  recenterOculusToolStripMenuItem;
private: System::Windows::Forms::Label^  label4;






























        // timer for graphic rendering
        System::Windows::Forms::Timer^  timer1;


#pragma region Windows Form Designer generated code
        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        void InitializeComponent(void)
        {
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->panelView = (gcnew System::Windows::Forms::Panel());
			this->groupBox = (gcnew System::Windows::Forms::GroupBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->menuStrip = (gcnew System::Windows::Forms::MenuStrip());
			this->menuToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resetViewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->recenterOculusToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->resetWorldToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->statusStrip = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->groupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			this->menuStrip->SuspendLayout();
			this->statusStrip->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->SuspendLayout();
			// 
			// panelView
			// 
			this->panelView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->panelView->ForeColor = System::Drawing::SystemColors::AppWorkspace;
			this->panelView->Location = System::Drawing::Point(217, 27);
			this->panelView->Name = L"panelView";
			this->panelView->Size = System::Drawing::Size(967, 809);
			this->panelView->TabIndex = 1;
			this->panelView->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::panelView_Paint);
			this->panelView->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::panelView_MouseDown);
			this->panelView->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::panelView_MouseMove);
			this->panelView->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::panelView_MouseUp);
			this->panelView->Resize += gcnew System::EventHandler(this, &MainForm::panelView_Resize);
			// 
			// groupBox
			// 
			this->groupBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->groupBox->AutoSize = true;
			this->groupBox->Controls->Add(this->label4);
			this->groupBox->Controls->Add(this->button5);
			this->groupBox->Controls->Add(this->button4);
			this->groupBox->Controls->Add(this->label6);
			this->groupBox->Controls->Add(this->label2);
			this->groupBox->Controls->Add(this->button2);
			this->groupBox->Controls->Add(this->button1);
			this->groupBox->Controls->Add(this->trackBar2);
			this->groupBox->Controls->Add(this->label3);
			this->groupBox->Controls->Add(this->trackBar1);
			this->groupBox->Controls->Add(this->label1);
			this->groupBox->Location = System::Drawing::Point(11, 87);
			this->groupBox->Name = L"groupBox";
			this->groupBox->Size = System::Drawing::Size(200, 298);
			this->groupBox->TabIndex = 2;
			this->groupBox->TabStop = false;
			this->groupBox->Text = L"Options for the selected object";
			this->groupBox->Enter += gcnew System::EventHandler(this, &MainForm::groupBox_Enter);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(6, 75);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(100, 13);
			this->label4->TabIndex = 23;
			this->label4->Text = L"Selected Organelle:";
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(119, 255);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->TabIndex = 22;
			this->button5->Text = L"Scale -5%";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &MainForm::button5_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(6, 255);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 23);
			this->button4->TabIndex = 21;
			this->button4->Text = L"Scale +5%";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MainForm::button4_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(6, 97);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(35, 13);
			this->label6->TabIndex = 19;
			this->label6->Text = L"label6";
			this->label6->Click += gcnew System::EventHandler(this, &MainForm::label6_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(6, 46);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(35, 13);
			this->label2->TabIndex = 17;
			this->label2->Text = L"label2";
			this->label2->Click += gcnew System::EventHandler(this, &MainForm::label2_Click_3);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(119, 20);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 16;
			this->button2->Text = L"Next";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MainForm::button2_Click);
			// 
			// button1
			// 
			this->button1->Enabled = false;
			this->button1->Location = System::Drawing::Point(7, 20);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 15;
			this->button1->Text = L"Previous";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// trackBar2
			// 
			this->trackBar2->Location = System::Drawing::Point(24, 204);
			this->trackBar2->Maximum = 9;
			this->trackBar2->Name = L"trackBar2";
			this->trackBar2->Size = System::Drawing::Size(147, 45);
			this->trackBar2->TabIndex = 14;
			this->trackBar2->Scroll += gcnew System::EventHandler(this, &MainForm::trackBar2_Scroll);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 188);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(72, 13);
			this->label3->TabIndex = 13;
			this->label3->Text = L"Transparency";
			this->label3->Click += gcnew System::EventHandler(this, &MainForm::label3_Click_3);
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(24, 140);
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(147, 45);
			this->trackBar1->TabIndex = 1;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &MainForm::trackBar1_Scroll);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 124);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(34, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Zoom";
			this->label1->Click += gcnew System::EventHandler(this, &MainForm::label1_Click);
			// 
			// menuStrip
			// 
			this->menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->menuToolStripMenuItem,
					this->viewToolStripMenuItem, this->helpToolStripMenuItem
			});
			this->menuStrip->Location = System::Drawing::Point(0, 0);
			this->menuStrip->Name = L"menuStrip";
			this->menuStrip->Size = System::Drawing::Size(1184, 24);
			this->menuStrip->TabIndex = 4;
			this->menuStrip->Text = L"menuStrip1";
			// 
			// menuToolStripMenuItem
			// 
			this->menuToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->exitToolStripMenuItem });
			this->menuToolStripMenuItem->Name = L"menuToolStripMenuItem";
			this->menuToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->menuToolStripMenuItem->Text = L"File";
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(92, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::exitToolStripMenuItem_Click);
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->resetViewToolStripMenuItem,
					this->recenterOculusToolStripMenuItem
			});
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(44, 20);
			this->viewToolStripMenuItem->Text = L"View";
			// 
			// resetViewToolStripMenuItem
			// 
			this->resetViewToolStripMenuItem->Name = L"resetViewToolStripMenuItem";
			this->resetViewToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->resetViewToolStripMenuItem->Text = L"Reset view";
			this->resetViewToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::resetViewToolStripMenuItem_Click);
			// 
			// recenterOculusToolStripMenuItem
			// 
			this->recenterOculusToolStripMenuItem->Name = L"recenterOculusToolStripMenuItem";
			this->recenterOculusToolStripMenuItem->Size = System::Drawing::Size(160, 22);
			this->recenterOculusToolStripMenuItem->Text = L"Recenter Oculus";
			this->recenterOculusToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::recenterOculusToolStripMenuItem_Click);
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->aboutToolStripMenuItem,
					this->resetWorldToolStripMenuItem
			});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(48, 20);
			this->helpToolStripMenuItem->Text = L"Tools";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(137, 22);
			this->aboutToolStripMenuItem->Text = L"Screenshot";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::aboutToolStripMenuItem_Click);
			// 
			// resetWorldToolStripMenuItem
			// 
			this->resetWorldToolStripMenuItem->Name = L"resetWorldToolStripMenuItem";
			this->resetWorldToolStripMenuItem->Size = System::Drawing::Size(137, 22);
			this->resetWorldToolStripMenuItem->Text = L"Reset World";
			this->resetWorldToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::resetWorldToolStripMenuItem_Click);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 10;
			this->timer1->Tick += gcnew System::EventHandler(this, &MainForm::timer1_Tick);
			// 
			// statusStrip
			// 
			this->statusStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->toolStripStatusLabel });
			this->statusStrip->Location = System::Drawing::Point(0, 839);
			this->statusStrip->Name = L"statusStrip";
			this->statusStrip->Size = System::Drawing::Size(1184, 22);
			this->statusStrip->TabIndex = 3;
			this->statusStrip->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel
			// 
			this->toolStripStatusLabel->Name = L"toolStripStatusLabel";
			this->toolStripStatusLabel->Size = System::Drawing::Size(12, 17);
			this->toolStripStatusLabel->Text = L"-";
			this->toolStripStatusLabel->Click += gcnew System::EventHandler(this, &MainForm::toolStripStatusLabel_Click);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Checked = true;
			this->radioButton2->Location = System::Drawing::Point(15, 22);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(56, 17);
			this->radioButton2->TabIndex = 5;
			this->radioButton2->TabStop = true;
			this->radioButton2->Text = L"Animal";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButton2_CheckedChanged);
			// 
			// groupBox1
			// 
			this->groupBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left));
			this->groupBox1->AutoSize = true;
			this->groupBox1->Controls->Add(this->radioButton3);
			this->groupBox1->Controls->Add(this->radioButton2);
			this->groupBox1->Location = System::Drawing::Point(11, 27);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(200, 58);
			this->groupBox1->TabIndex = 7;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Virtual cell model";
			this->groupBox1->Enter += gcnew System::EventHandler(this, &MainForm::groupBox1_Enter);
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Location = System::Drawing::Point(111, 22);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(49, 17);
			this->radioButton3->TabIndex = 6;
			this->radioButton3->Text = L"Plant";
			this->radioButton3->UseVisualStyleBackColor = true;
			this->radioButton3->CheckedChanged += gcnew System::EventHandler(this, &MainForm::radioButton3_CheckedChanged);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1184, 861);
			this->Controls->Add(this->groupBox);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->statusStrip);
			this->Controls->Add(this->menuStrip);
			this->Controls->Add(this->panelView);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MainMenuStrip = this->menuStrip;
			this->Name = L"MainForm";
			this->Text = L"Virtual Microscope";
			this->groupBox->ResumeLayout(false);
			this->groupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			this->menuStrip->ResumeLayout(false);
			this->menuStrip->PerformLayout();
			this->statusStrip->ResumeLayout(false);
			this->statusStrip->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

    //--------------------------------------------------------------------------
    // CHAI3D - METHODS
    //--------------------------------------------------------------------------
    private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e)
    {
        UNREFERENCED_PARAMETER(sender);
        UNREFERENCED_PARAMETER(e);

        // update status bar
        this->toolStripStatusLabel->Text = String::Format("Graphics: {0} Hz   Haptics: {1} Hz", 
                                        this->application->m_graphicRate.getFrequency(), 
                                        this->application->m_hapticRate.getFrequency());
				 
		// Display the phase of the simulation
		if (this->application->m_simulationPhase == this->application->PHASE1)
			this->label2->Text = "Phase 1";
		else if (this->application->m_simulationPhase == this->application->PHASE2)
			this->label2->Text = "Phase 2";
		else if (this->application->m_simulationPhase == this->application->PHASE3)
			this->label2->Text = "Phase 3";
		else
			this->label2->Text = "Phase -";

		// Display the label of the selected organelle
		std::string selectedObjectName = this->application->organelleName;
		this->label6->Text = gcnew String(selectedObjectName.c_str());

		// Display transparency level
		this->label3->Text = String::Format("Transparency: {0}%", 100 - (1.0 - (double)(this->trackBar2->Value)/10) * 100);

		// Reset transparence level when selecting a new object
		if (this->application->selectedObject == NULL)
			this->trackBar2->Value = 0;
		//else
			//this->application->selectedObject->m_texture->m_image->

		// Check for the selected object and enable/disable NEXT button
		if (this->application->m_simulationPhase != this->application->PHASE1 && this->application->selectedObject == NULL)
			this->button2->Enabled = false;
		else
			this->button2->Enabled = true;

        // render scene
        viewport->preRender();
        application->paintGL();
        viewport->postRender();
    }

    private: System::Void panelView_Resize(System::Object^  sender, System::EventArgs^  e) 
    {
        int w = this->panelView->Size.Width;
        int h = this->panelView->Size.Height;
        this->application->resizeGL(w, h);
    }

    private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
    {
        Close();
    }

    private: System::Void trackBar1_Scroll(System::Object^  sender, System::EventArgs^  e) 
    {
        double value = 0.5 * (double)(this->trackBar1->Value);
        this->application->m_camera->setSphericalRadius(value);
		this->application->m_cameraOculus->setSphericalRadius(value);
    }

    private: System::Void panelView_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
    {
        moveCamera = true;
        mouseX = e->X;
        mouseY = e->Y;
    }

    private: System::Void panelView_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
    {
        if (moveCamera)
        {
        // compute mouse motion
        int dx = e->X - mouseX;
        int dy = e->Y - mouseY;
        mouseX = e->X;
        mouseY = e->Y;

        // compute new camera angles
        double azimuthDeg = this->application->m_camera->getSphericalAzimuthDeg() + (0.5 * dy);
        double polarDeg = this->application->m_camera->getSphericalPolarDeg() + (-0.5 * dx);

        // assign new angles
        this->application->m_camera->setSphericalAzimuthDeg(azimuthDeg);
        this->application->m_camera->setSphericalPolarDeg(polarDeg);
		this->application->m_cameraOculus->setSphericalAzimuthDeg(azimuthDeg);
		this->application->m_cameraOculus->setSphericalPolarDeg(polarDeg);

        // line up tool with camera
        this->application->m_tool->setLocalRot(this->application->m_camera->getLocalRot());
        }
    }

    private: System::Void panelView_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
    {
        moveCamera = false;
    }

	private: System::Void viewToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void groupBox_Enter(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label2_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label3_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void fullScreenToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label4_Click(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void groupBox1_Enter(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void label3_Click_1(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		// Switch to animal cell model
		std::vector<chai3d::cMultiMesh*>::const_iterator it;
		chai3d::cMultiMesh* tmpObject;

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); ++it)
		{
			tmpObject = *it;
			tmpObject->setEnabled(false);
		}

		this->application->currentCellVector.clear();
		
		if (this->application->m_simulationPhase == this->application->PHASE1)
			this->application->currentCellVector = this->application->multipleAnimalCell;
		else if (this->application->m_simulationPhase == this->application->PHASE2)
			this->application->currentCellVector = this->application->singleAnimalCell;

		this->application->m_cellType = this->application->ANIMAL;

		this->application->background->loadFromFile("C:/3DLearning/images/background/background_animal.jpg");
		
		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); ++it)
		{
			tmpObject = *it;
			tmpObject->setEnabled(true);
		}
	}

	private: System::Void radioButton3_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		// Switch to plant cell model
		std::vector<chai3d::cMultiMesh*>::const_iterator it;
		chai3d::cMultiMesh* tmpObject;

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); ++it)
		{
			tmpObject = *it;
			tmpObject->setEnabled(false);
		}

		this->application->currentCellVector.clear();
		
		if (this->application->m_simulationPhase == this->application->PHASE1)
			this->application->currentCellVector = this->application->multiplePlantCell;
		else if (this->application->m_simulationPhase == this->application->PHASE2)
			this->application->currentCellVector = this->application->singlePlantCell;

		this->application->m_cellType = this->application->PLANT;
		
		this->application->background->loadFromFile("C:/3DLearning/images/background/background_plant3.jpg");

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); ++it)
		{
			tmpObject = *it;
			tmpObject->setEnabled(true);
		}
	}

	private: System::Void panelView_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
	}
	
	private: System::Void label3_Click_2(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void toolStripStatusLabel_Click(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void label3_Click_3(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void trackBar2_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		// Change the transparency of the selected object
		if (this->application->selectedObject != NULL)
			this->application->selectedObject->setTransparencyLevel(1.0 - (double)(this->trackBar2->Value) / 10);
		else
			this->trackBar2->Value = 0;
	}

	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
	{
		// Proceed to the NEXT Phase

		std::vector<chai3d::cMultiMesh*>::const_iterator it;
		chai3d::cMultiMesh* tmpObject;

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); ++it)
		{
			tmpObject = *it;
			tmpObject->setEnabled(false);
		}

		if (this->application->selectedObject != NULL)
			this->application->selectedObject->setShowBoundaryBox(false);

		//this->application->currentCellVector.clear();

		if (this->application->m_simulationPhase == this->application->PHASE1)
		{

			if (this->application->m_cellType == this->application->ANIMAL)
			{
				this->application->currentCellVector = this->application->singleAnimalCell;
			}
			else
			{
				this->application->currentCellVector = this->application->singlePlantCell;
			}

			this->application->m_simulationPhase = this->application->PHASE2;
			this->button1->Enabled = true;
			this->button2->Enabled = false;

		}
		else if (this->application->m_simulationPhase == this->application->PHASE2)
		{
			if (this->application->m_cellType == this->application->ANIMAL)
			{
				if (this->application->selectedObject->m_name == "NuclearEnvelope")
				{
					this->application->currentCellVector = this->application->animalNucleus;
				}
				else if (this->application->selectedObject->m_name == "Golgi")
				{
					this->application->currentCellVector = this->application->animalGolgi;
				}
				else if (this->application->selectedObject->m_name == "SmoothEndoplasmicRecticulum" || this->application->selectedObject->m_name == "RoughEndoplasmicRecticulum")
				{
					this->application->currentCellVector = this->application->animalEndoplasmicReticulum;
				}
				else if (this->application->selectedObject->m_name == "Peroxisome")
				{
					this->application->currentCellVector = this->application->animalPeroxisome;
				}
				else if (this->application->selectedObject->m_name == "Peroxisome")
				{
					this->application->currentCellVector = this->application->animalPeroxisome;
				}
				else if (this->application->selectedObject->m_name == "Lysosome")
				{
					this->application->currentCellVector = this->application->animalLysosome;
				}
				else if (this->application->selectedObject->m_name == "MitochondrionInside" || this->application->selectedObject->m_name == "Mitochondrion")
				{
					this->application->currentCellVector = this->application->animalMitochondrion;
				}
			}
			else
			{
				if (this->application->selectedObject->m_name == "ThylakoidMembrane")
				{
					this->application->currentCellVector = this->application->plantChloroplast;
				}
				else if (this->application->selectedObject->m_name == "Vacuole")
				{
					this->application->currentCellVector = this->application->plantVacoule;
				}
				else if (this->application->selectedObject->m_name == "ActinFilaments")
				{
					this->application->currentCellVector = this->application->plantActinFilaments;
				}
				else if (this->application->selectedObject->m_name == "SmoothEndoplasmicReticulum" || this->application->selectedObject->m_name == "RoughEndoplasmicReticulum")
				{
					this->application->currentCellVector = this->application->plantEndoplasmicReticulum;
				}
				else if (this->application->selectedObject->m_name == "Peroxisome")
				{
					this->application->currentCellVector = this->application->plantPeroxisome;
				}
				else if (this->application->selectedObject->m_name == "Nucleus")
				{
					this->application->currentCellVector = this->application->plantNucleus;
				}
				else if (this->application->selectedObject->m_name == "Mitochondrion")
				{
					this->application->currentCellVector = this->application->plantMitochondrion;
				}
				else if (this->application->selectedObject->m_name == "Golgi")
				{
					this->application->currentCellVector = this->application->plantGolgi;
				}
			}

			this->application->m_simulationPhase = this->application->PHASE3;
			this->button1->Enabled = true;
			this->button2->Enabled = false;
		}

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); ++it)
		{
			tmpObject = *it;
			tmpObject->setEnabled(true);
		}

		this->application->selectedObject = NULL;
	}

	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
	{
		// Go back to the PREVIOUS Phase

		std::vector<chai3d::cMultiMesh*>::const_iterator it;
		chai3d::cMultiMesh* tmpObject;

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); ++it)
		{
			tmpObject = *it;
			tmpObject->setEnabled(false);
		}

		this->application->currentCellVector.clear();

		if (this->application->m_simulationPhase == this->application->PHASE3)
		{
			if (this->application->m_cellType == this->application->ANIMAL)
			{
				this->application->currentCellVector = this->application->singleAnimalCell;
			}
			else
			{
				this->application->currentCellVector = this->application->singlePlantCell;
			}

			this->application->m_simulationPhase = this->application->PHASE2;
			this->button1->Enabled = true;
			this->button2->Enabled = true;
		}
		else if (this->application->m_simulationPhase == this->application->PHASE2)
		{
			if (this->application->m_cellType == this->application->ANIMAL)
			{
				this->application->currentCellVector = this->application->multipleAnimalCell;
			}
			else
			{
				this->application->currentCellVector = this->application->multiplePlantCell;
			}

			this->application->m_simulationPhase = this->application->PHASE1;
			this->button1->Enabled = false;
			this->button2->Enabled = true;
		}

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); ++it)
		{
			tmpObject = *it;
			tmpObject->setEnabled(true);
		}

		this->application->selectedObject = NULL;
	}

	private: System::Void label2_Click_3(System::Object^  sender, System::EventArgs^  e) { }

	private: System::Void label6_Click(System::Object^  sender, System::EventArgs^  e) { }
	
	private: System::Void label5_Click(System::Object^  sender, System::EventArgs^  e) { }

	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e)
	{
	}

	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e)
	{
		chai3d::cMultiMesh* tmpObject;
		std::vector<chai3d::cMultiMesh*>::const_iterator it;

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); it++)
		{
			tmpObject = *it;
			tmpObject->setHapticEnabled(false);
		}

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); it++)
		{
			tmpObject = *it;
			if (tmpObject->getEnabled())
			{
				tmpObject->scale(1.05);
				tmpObject->createAABBCollisionDetector(this->application->m_toolRadius);
			}
		}

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); it++)
		{
			tmpObject = *it;
			tmpObject->setHapticEnabled(true);
		}
	}
	private: System::Void button5_Click(System::Object^  sender, System::EventArgs^  e)
	{
		chai3d::cMultiMesh* tmpObject;
		std::vector<chai3d::cMultiMesh*>::const_iterator it;

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); it++)
		{
			tmpObject = *it;
			tmpObject->setHapticEnabled(false);
		}

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); it++)
		{
			tmpObject = *it;
			if (tmpObject->getEnabled())
			{
				tmpObject->scale(0.95);
				tmpObject->createAABBCollisionDetector(this->application->m_toolRadius);
			}
		}

		for (it = this->application->currentCellVector.begin(); it != this->application->currentCellVector.end(); it++)
		{
			tmpObject = *it;
			tmpObject->setHapticEnabled(true);
		}
	}

	private: System::Void resetViewToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		this->application->m_camera->setSphericalReferences(chai3d::cVector3d(0, 0, 0),    // origin
			chai3d::cVector3d(0, 0, 1),    // zenith direction
			chai3d::cVector3d(1, 0, 0));   // azimuth direction

		this->application->m_cameraOculus->setSphericalReferences(chai3d::cVector3d(0, 0, 0),    // origin
			chai3d::cVector3d(0, 0, 1),    // zenith direction
			chai3d::cVector3d(1, 0, 0));   // azimuth direction

		this->application->m_camera->setSphericalDeg(2.0,    // spherical coordinate radius
			0,     // spherical coordinate azimuth angle
			0);    // spherical coordinate polar angle

		this->application->m_cameraOculus->setSphericalDeg(2.0,    // spherical coordinate radius
			0,     // spherical coordinate azimuth angle
			0);    // spherical coordinate polar angle
	}

	private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		chai3d::cImagePtr image = chai3d::cImage::create();
		this->application->m_camera->copyImageBuffer(image);
		image->saveToFile("screenshot_" + this->application->currentDateTime() + ".png");
	}

	private: System::Void resetWorldToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		chai3d::cMatrix3d localRot(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
		chai3d::cVector3d localPos(0.0, 0.0, 0.0);
		/*this->application->selectedObject->getChild(0)->setLocalRot(localRot);
		this->application->selectedObject->getChild(0)->setLocalPos(localPos);*/

		//chai3d::cGenericObject* tmpMesh;
		std::vector<chai3d::cMultiMesh*>::const_iterator it;
		for (it = this->application->singleAnimalCell.begin(); it != this->application->singleAnimalCell.end(); it++)
		{
			(*it)->setLocalRot(localRot);
			(*it)->setLocalPos(localPos);
		}
	}
private: System::Void recenterOculusToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	this->application->m_oculusVR.recenterPose();
}

};
}
