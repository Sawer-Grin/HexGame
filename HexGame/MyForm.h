#pragma once
#include "main_data_first.h"
#include "main_data.h"
#include "controller.h"
#include "method_monte.h"

const int delta_y = 8;

namespace HexGame {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;
	controller main_controller;
	method_monte monte_carlo_AI;

	/// <summary>
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
		}

		void initialize_field()
		{

			int temp_x = size_of_picture_field.Width / 2 - (size_of_field * size_picture_x) / 2;
			int temp_y = size_of_picture_field.Height / 2 - size_picture_y / 2;
			Point start_point(temp_x, temp_y);
			Point copy_start_point = start_point;
			int start_index = size_of_field * (size_of_field - 1);

			this->pictures = (gcnew array<System::Windows::Forms::PictureBox^>(size_of_field * size_of_field));
			this->border_pictures = (gcnew System::Windows::Forms::PictureBox());
			initialize_border_pic();
			Bitmap^ img = gcnew Bitmap(size_of_picture_field.Width, size_of_picture_field.Height);
			Graphics^ temp_g = Graphics::FromImage(img);
			temp_g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;
			

			for (int i = 0; i < this->size_of_field; ++i)
			{
				drawing_one_line(start_point, start_index, temp_g);
				start_point.X = start_point.X + size_picture_x / 2;
				start_point.Y = start_point.Y - size_picture_y - delta_y;
				start_index -= size_of_field;
			}

			draw_for_stuped_player(copy_start_point, temp_g);
			delete temp_g;
			this->border_pictures->Image = img;
			this->Controls->Add(this->border_pictures);

		}

		// in method exists data such as size_picture_.., you can find this in main_data.h
		void generation_field(const setting_of_game& setting)
		{
			//this->computer = setting.computer;
			main_controller = controller(setting.size_of_bord, setting.first_player);
			initialize_field();
			// customization  AI
			if (setting.method != none_method)
			{
				if (setting.method == monte_carlo_method)
				{
					monte_carlo_AI = method_monte((hex_gamer)setting.computer, setting.size_of_bord);
				}
				else if ((setting.method == minmax_method) || (setting.method == alpha_beta_method)){
					///  add tree_search
					int second = 0;
				}
			}
		}

		void draw_for_stuped_player(const Point& start, Graphics^ temp_g)
		{
			Point local_start = start;
			Pen^ first_pen = gcnew Pen(Color::Blue, 2);
			Pen^ second_pen = gcnew Pen(Color::Red, 2);

			// I draw the second sector (from trigometry)
			PointF first_point = PointF(local_start.X, local_start.Y + size_picture_y / 2);
			PointF second_point = PointF(local_start.X, local_start.Y);
			PointF third_point = second_point;
			temp_g->DrawLine(first_pen, first_point, second_point);
			for (int i = 0; i < this->size_of_field - 1; ++i)
			{
				first_point = third_point;
				second_point = PointF(first_point.X + size_picture_x / 2, first_point.Y - delta_y);
				temp_g->DrawLine(first_pen, first_point, second_point);
				third_point = PointF(second_point.X, second_point.Y - size_picture_y);
				temp_g->DrawLine(first_pen, second_point, third_point);
			}
			first_point = PointF(third_point.X + size_picture_x / 2, third_point.Y - delta_y);
			temp_g->DrawLine(first_pen, third_point, first_point);

			// I draw the first sector (from trigometry)
			second_point = PointF(first_point.X + size_picture_x / 2, first_point.Y + delta_y);
			temp_g->DrawLine(second_pen, first_point, second_point);
			third_point = second_point;
			for (int i = 0; i < this->size_of_field - 1; ++i)
			{
				first_point = third_point;
				second_point = PointF(first_point.X, first_point.Y + size_picture_y);
				temp_g->DrawLine(second_pen, first_point, second_point);
				third_point = PointF(second_point.X + size_picture_x / 2, second_point.Y + delta_y);
				temp_g->DrawLine(second_pen, second_point, third_point);
			}
			first_point = PointF(third_point.X, third_point.Y + size_picture_y / 2);
			temp_g->DrawLine(second_pen, third_point, first_point);

			// I draw the fourth sector (from trigometry)
			second_point = PointF(first_point.X, first_point.Y + size_picture_y / 2);
			temp_g->DrawLine(first_pen, first_point, second_point);
			third_point = second_point;
			for (int i = 0; i < this->size_of_field - 1; ++i)
			{
				first_point = third_point;
				second_point = PointF(first_point.X - size_picture_x / 2, first_point.Y + delta_y);
				temp_g->DrawLine(first_pen, first_point, second_point);
				third_point = PointF(second_point.X, second_point.Y + size_picture_y);
				temp_g->DrawLine(first_pen, second_point, third_point);
			}
			first_point = PointF(third_point.X - size_picture_x / 2, third_point.Y + delta_y);
			temp_g->DrawLine(first_pen, third_point, first_point);

			// I draw the fourth sector (from trigometry)
			second_point = PointF(first_point.X - size_picture_x / 2, first_point.Y - delta_y);
			temp_g->DrawLine(second_pen, first_point, second_point);
			third_point = second_point;

			for (int i = 0; i < this->size_of_field - 1; ++i)
			{
				first_point = third_point;
				second_point = PointF(first_point.X, first_point.Y - size_picture_y);
				temp_g->DrawLine(second_pen, first_point, second_point);
				third_point = PointF(second_point.X - size_picture_x / 2, second_point.Y - delta_y);
				temp_g->DrawLine(second_pen, second_point, third_point);
			}
			first_point = PointF(third_point.X, third_point.Y - size_picture_y / 2);
			temp_g->DrawLine(second_pen, third_point, first_point);

		}

		void delete_field()
		{
			for (int i = 0; i < this->size_of_field; ++i)
				this->border_pictures->Controls->Remove(pictures[i]);
			delete border_pictures;
		}

		void initialize_border_pic()
		{
			this->border_pictures = (gcnew System::Windows::Forms::PictureBox());
			this->border_pictures->Location = position_of_picture_field; 
			this->border_pictures->Size = size_of_picture_field;
			this->border_pictures->Name = L"border_pictures";
			this->border_pictures->TabIndex = 3;
			this->border_pictures->TabStop = false;
			this->border_pictures->BackColor = System::Drawing::Color::White;
		}

		void drawing_one_line(const Point& start, const int& start_index, Graphics^ temp_g)
		{			
			int local_index = start_index;
			Point local_start = start;
			int index = 0;
			Pen^ pen_border = gcnew Pen(Color::Black, 2);

			for (int i = 0; i < this->size_of_field; ++i)
			{
				// draw hex 
				this->pictures[local_index] = gcnew System::Windows::Forms::PictureBox();
				this->pictures[local_index]->ImageLocation = "hex_t.gif";
				this->pictures[local_index]->Location = Point(local_start.X + 2, local_start.Y);
				this->pictures[local_index]->Name = local_index.ToString();
				this->pictures[local_index]->Size = System::Drawing::Size(28, 24);
				this->pictures[local_index]->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
				this->pictures[local_index]->TabIndex = 0;
				this->pictures[local_index]->TabStop = false;
				this->pictures[local_index]->BackColor = System::Drawing::Color::White;
				this->pictures[local_index]->Click += gcnew System::EventHandler(this, &MyForm::hex_Click_1);

				// draw border field
				PointF first_point = PointF(local_start.X, local_start.Y);
				PointF second_point = PointF(local_start.X, local_start.Y + size_picture_y);
				PointF third_point = PointF(local_start.X + size_picture_x / 2, local_start.Y + size_picture_y + delta_y);
				PointF fourth_point = PointF(local_start.X + size_picture_x , local_start.Y + size_picture_y);
				PointF fifth_point = PointF(local_start.X + size_picture_x, local_start.Y);
				PointF sixth_point = PointF(local_start.X + size_picture_x / 2, local_start.Y - delta_y);
				array<PointF>^ curve_points = { first_point, second_point, third_point, fourth_point, fifth_point, sixth_point };
				Pen^ pen_border = gcnew Pen(Color::Black, 2);
				temp_g->DrawPolygon(pen_border, curve_points);

				this->border_pictures->Controls->Add(pictures[local_index]);
				local_start = Point((int)third_point.X, (int)third_point.Y);
				++local_index;
			}
		}

	protected:
		/// <summary>
		/// Îñâîáîäèòü âñå èñïîëüçóåìûå ðåñóðñû.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Panel^ panel2;
	private: System::Windows::Forms::Panel^ panel5;

	private: System::Windows::Forms::Panel^ panel3;

	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::RadioButton^ Monte_Carlo;
	private: System::Windows::Forms::RadioButton^ MinMax;
	private: System::Windows::Forms::RadioButton^ Alpha_beta;
	private: CustomControls::RJControls::RJButton^ button_for_generate;
	private: System::ComponentModel::IContainer^ components;
	private: array<System::Windows::Forms::PictureBox^>^ pictures;
	private: System::Windows::Forms::PictureBox^ border_pictures;
	private: System::Windows::Forms::RadioButton^ but_none_pl;
	private: System::Windows::Forms::RadioButton^ but_second_pl;
	private: System::Windows::Forms::RadioButton^ but_first_pl;
	private: System::Windows::Forms::Panel^ panel4;
	private: System::Windows::Forms::Label^ label2;

	private: System::Windows::Forms::RadioButton^ second_turn;
	private: System::Windows::Forms::RadioButton^ turn_first;

	private: hex_gamer computer = none;
	private: hex_gamer walks_first = first;
	private: method_of_AI method = alpha_beta_method;
	private: Point position_of_picture_field =  System::Drawing::Point(136, 12);
	private: System::Drawing::Size size_of_picture_field = System::Drawing::Size(469, 700);
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::ComboBox^ comboBox1;
	private: System::Windows::Forms::TextBox^ tablo_player;
	private: System::Windows::Forms::Panel^ panel_end_of_game;
	private: System::Windows::Forms::Label^ tablo_winner;
	private: CustomControls::RJControls::RJButton^ button_for_next_game;
	private: CustomControls::RJControls::RJButton^ button_exit;

	private: int size_of_field = 3;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// </summary>
		void InitializeComponent(void)
		{
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->tablo_player = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->panel5 = (gcnew System::Windows::Forms::Panel());
			this->button_for_generate = (gcnew CustomControls::RJControls::RJButton());
			this->panel4 = (gcnew System::Windows::Forms::Panel());
			this->second_turn = (gcnew System::Windows::Forms::RadioButton());
			this->turn_first = (gcnew System::Windows::Forms::RadioButton());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->Monte_Carlo = (gcnew System::Windows::Forms::RadioButton());
			this->MinMax = (gcnew System::Windows::Forms::RadioButton());
			this->Alpha_beta = (gcnew System::Windows::Forms::RadioButton());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->but_none_pl = (gcnew System::Windows::Forms::RadioButton());
			this->but_second_pl = (gcnew System::Windows::Forms::RadioButton());
			this->but_first_pl = (gcnew System::Windows::Forms::RadioButton());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->panel_end_of_game = (gcnew System::Windows::Forms::Panel());
			this->button_exit = (gcnew CustomControls::RJControls::RJButton());
			this->button_for_next_game = (gcnew CustomControls::RJControls::RJButton());
			this->tablo_winner = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->panel1->SuspendLayout();
			this->panel5->SuspendLayout();
			this->panel4->SuspendLayout();
			this->panel3->SuspendLayout();
			this->panel2->SuspendLayout();
			this->panel_end_of_game->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::Color::NavajoWhite;
			this->panel1->Controls->Add(this->tablo_player);
			this->panel1->Controls->Add(this->label5);
			this->panel1->Controls->Add(this->comboBox1);
			this->panel1->Controls->Add(this->panel5);
			this->panel1->Controls->Add(this->panel4);
			this->panel1->Controls->Add(this->panel3);
			this->panel1->Controls->Add(this->panel2);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->panel1->Location = System::Drawing::Point(769, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(353, 708);
			this->panel1->TabIndex = 0;
			// 
			// tablo_player
			// 
			this->tablo_player->BackColor = System::Drawing::Color::PapayaWhip;
			this->tablo_player->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->tablo_player->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->tablo_player->Location = System::Drawing::Point(11, 11);
			this->tablo_player->Name = L"tablo_player";
			this->tablo_player->ReadOnly = true;
			this->tablo_player->Size = System::Drawing::Size(100, 21);
			this->tablo_player->TabIndex = 23;
			this->tablo_player->Visible = false;
			this->tablo_player->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(22, 476);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(162, 24);
			this->label5->TabIndex = 22;
			this->label5->Text = L"Amount of players";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(6) { L"4", L"5", L"7", L"9", L"10", L"11" });
			this->comboBox1->Location = System::Drawing::Point(196, 472);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 28);
			this->comboBox1->TabIndex = 5;
			this->comboBox1->Text = L"3";
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// panel5
			// 
			this->panel5->BackColor = System::Drawing::Color::NavajoWhite;
			this->panel5->Controls->Add(this->button_for_generate);
			this->panel5->Location = System::Drawing::Point(29, 607);
			this->panel5->Name = L"panel5";
			this->panel5->Size = System::Drawing::Size(291, 81);
			this->panel5->TabIndex = 1;
			// 
			// button_for_generate
			// 
			this->button_for_generate->BackColor = System::Drawing::Color::MediumSlateBlue;
			this->button_for_generate->BackgroundColor = System::Drawing::Color::MediumSlateBlue;
			this->button_for_generate->BorderColor = System::Drawing::Color::PaleVioletRed;
			this->button_for_generate->BorderRadius = 20;
			this->button_for_generate->BorderSize = 0;
			this->button_for_generate->FlatAppearance->BorderSize = 0;
			this->button_for_generate->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button_for_generate->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button_for_generate->ForeColor = System::Drawing::Color::Black;
			this->button_for_generate->Location = System::Drawing::Point(60, 15);
			this->button_for_generate->Name = L"button_for_generate";
			this->button_for_generate->Size = System::Drawing::Size(161, 52);
			this->button_for_generate->TabIndex = 4;
			this->button_for_generate->Text = L"Start";
			this->button_for_generate->TextColor = System::Drawing::Color::Black;
			this->button_for_generate->UseVisualStyleBackColor = false;
			this->button_for_generate->Click += gcnew System::EventHandler(this, &MyForm::generation_Click_1);
			// 
			// panel4
			// 
			this->panel4->BackColor = System::Drawing::Color::NavajoWhite;
			this->panel4->Controls->Add(this->second_turn);
			this->panel4->Controls->Add(this->turn_first);
			this->panel4->Controls->Add(this->label2);
			this->panel4->Location = System::Drawing::Point(29, 324);
			this->panel4->Name = L"panel4";
			this->panel4->Size = System::Drawing::Size(291, 114);
			this->panel4->TabIndex = 1;
			// 
			// second_turn
			// 
			this->second_turn->AutoSize = true;
			this->second_turn->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->second_turn->Location = System::Drawing::Point(45, 69);
			this->second_turn->Name = L"second_turn";
			this->second_turn->Size = System::Drawing::Size(52, 24);
			this->second_turn->TabIndex = 21;
			this->second_turn->TabStop = true;
			this->second_turn->Text = L"Red";
			this->second_turn->UseVisualStyleBackColor = true;
			this->second_turn->CheckedChanged += gcnew System::EventHandler(this, &MyForm::second_turn_CheckedChanged);
			// 
			// turn_first
			// 
			this->turn_first->AutoSize = true;
			this->turn_first->Checked = true;
			this->turn_first->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->turn_first->Location = System::Drawing::Point(45, 39);
			this->turn_first->Name = L"turn_first";
			this->turn_first->Size = System::Drawing::Size(56, 24);
			this->turn_first->TabIndex = 21;
			this->turn_first->TabStop = true;
			this->turn_first->Text = L"Blue";
			this->turn_first->UseVisualStyleBackColor = true;
			this->turn_first->CheckedChanged += gcnew System::EventHandler(this, &MyForm::turn_first_CheckedChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(97, 12);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(100, 24);
			this->label2->TabIndex = 14;
			this->label2->Text = L"First move:";
			this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click_1);
			// 
			// panel3
			// 
			this->panel3->BackColor = System::Drawing::Color::NavajoWhite;
			this->panel3->Controls->Add(this->Monte_Carlo);
			this->panel3->Controls->Add(this->MinMax);
			this->panel3->Controls->Add(this->Alpha_beta);
			this->panel3->Controls->Add(this->label6);
			this->panel3->Location = System::Drawing::Point(26, 177);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(291, 141);
			this->panel3->TabIndex = 1;
			// 
			// Monte_Carlo
			// 
			this->Monte_Carlo->AutoSize = true;
			this->Monte_Carlo->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Monte_Carlo->Location = System::Drawing::Point(45, 95);
			this->Monte_Carlo->Name = L"Monte_Carlo";
			this->Monte_Carlo->Size = System::Drawing::Size(172, 24);
			this->Monte_Carlo->TabIndex = 17;
			this->Monte_Carlo->Text = L"Monte-Carlo method";
			this->Monte_Carlo->UseVisualStyleBackColor = true;
			this->Monte_Carlo->CheckedChanged += gcnew System::EventHandler(this, &MyForm::Monte_Carlo_CheckedChanged);
			// 
			// MinMax
			// 
			this->MinMax->AutoSize = true;
			this->MinMax->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->MinMax->Location = System::Drawing::Point(45, 65);
			this->MinMax->Name = L"MinMax";
			this->MinMax->Size = System::Drawing::Size(157, 24);
			this->MinMax->TabIndex = 16;
			this->MinMax->Text = L"MinMax Algorithm";
			this->MinMax->UseVisualStyleBackColor = true;
			this->MinMax->CheckedChanged += gcnew System::EventHandler(this, &MyForm::MinMax_CheckedChanged);
			// 
			// Alpha_beta
			// 
			this->Alpha_beta->AutoSize = true;
			this->Alpha_beta->Checked = true;
			this->Alpha_beta->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Alpha_beta->Location = System::Drawing::Point(45, 36);
			this->Alpha_beta->Name = L"Alpha_beta";
			this->Alpha_beta->Size = System::Drawing::Size(179, 24);
			this->Alpha_beta->TabIndex = 1;
			this->Alpha_beta->TabStop = true;
			this->Alpha_beta->Text = L"Alpha-Beta Algorithm";
			this->Alpha_beta->UseVisualStyleBackColor = true;
			this->Alpha_beta->CheckedChanged += gcnew System::EventHandler(this, &MyForm::Alpha_bata_CheckedChanged);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->BackColor = System::Drawing::Color::NavajoWhite;
			this->label6->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(76, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(137, 22);
			this->label6->TabIndex = 15;
			this->label6->Text = L"Algorithm for AI:";
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::Color::NavajoWhite;
			this->panel2->Controls->Add(this->but_none_pl);
			this->panel2->Controls->Add(this->but_second_pl);
			this->panel2->Controls->Add(this->but_first_pl);
			this->panel2->Controls->Add(this->label3);
			this->panel2->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->panel2->Location = System::Drawing::Point(26, 49);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(291, 122);
			this->panel2->TabIndex = 11;
			// 
			// but_none_pl
			// 
			this->but_none_pl->AutoSize = true;
			this->but_none_pl->Checked = true;
			this->but_none_pl->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->but_none_pl->Location = System::Drawing::Point(45, 83);
			this->but_none_pl->Name = L"but_none_pl";
			this->but_none_pl->Size = System::Drawing::Size(65, 24);
			this->but_none_pl->TabIndex = 20;
			this->but_none_pl->TabStop = true;
			this->but_none_pl->Text = L"None";
			this->but_none_pl->UseVisualStyleBackColor = true;
			this->but_none_pl->CheckedChanged += gcnew System::EventHandler(this, &MyForm::but_none_pl_CheckedChanged);
			// 
			// but_second_pl
			// 
			this->but_second_pl->AutoSize = true;
			this->but_second_pl->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->but_second_pl->Location = System::Drawing::Point(45, 62);
			this->but_second_pl->Name = L"but_second_pl";
			this->but_second_pl->Size = System::Drawing::Size(74, 24);
			this->but_second_pl->TabIndex = 19;
			this->but_second_pl->TabStop = true;
			this->but_second_pl->Text = L"Second";
			this->but_second_pl->UseVisualStyleBackColor = true;
			this->but_second_pl->CheckedChanged += gcnew System::EventHandler(this, &MyForm::but_second_pl_CheckedChanged);
			// 
			// but_first_pl
			// 
			this->but_first_pl->AutoSize = true;
			this->but_first_pl->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->but_first_pl->Location = System::Drawing::Point(45, 41);
			this->but_first_pl->Name = L"but_first_pl";
			this->but_first_pl->Size = System::Drawing::Size(55, 24);
			this->but_first_pl->TabIndex = 18;
			this->but_first_pl->TabStop = true;
			this->but_first_pl->Text = L"First";
			this->but_first_pl->UseVisualStyleBackColor = true;
			this->but_first_pl->CheckedChanged += gcnew System::EventHandler(this, &MyForm::but_first_pl_CheckedChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 13, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(97, 15);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(100, 23);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Who is AI \?";
			this->label3->Click += gcnew System::EventHandler(this, &MyForm::label3_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(84, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(162, 27);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Setting for game:";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// panel_end_of_game
			// 
			this->panel_end_of_game->BackColor = System::Drawing::Color::PapayaWhip;
			this->panel_end_of_game->Controls->Add(this->button_exit);
			this->panel_end_of_game->Controls->Add(this->button_for_next_game);
			this->panel_end_of_game->Controls->Add(this->tablo_winner);
			this->panel_end_of_game->Location = System::Drawing::Point(754, 55);
			this->panel_end_of_game->Name = L"panel_end_of_game";
			this->panel_end_of_game->Size = System::Drawing::Size(389, 631);
			this->panel_end_of_game->TabIndex = 0;
			this->panel_end_of_game->Visible = false;
			// 
			// button_exit
			// 
			this->button_exit->BackColor = System::Drawing::Color::MediumSlateBlue;
			this->button_exit->BackgroundColor = System::Drawing::Color::MediumSlateBlue;
			this->button_exit->BorderColor = System::Drawing::Color::PaleVioletRed;
			this->button_exit->BorderRadius = 20;
			this->button_exit->BorderSize = 0;
			this->button_exit->FlatAppearance->BorderSize = 0;
			this->button_exit->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button_exit->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button_exit->ForeColor = System::Drawing::Color::White;
			this->button_exit->Location = System::Drawing::Point(126, 422);
			this->button_exit->Name = L"button_exit";
			this->button_exit->Size = System::Drawing::Size(151, 51);
			this->button_exit->TabIndex = 2;
			this->button_exit->Text = L"Exit";
			this->button_exit->TextColor = System::Drawing::Color::White;
			this->button_exit->UseVisualStyleBackColor = false;
			this->button_exit->Click += gcnew System::EventHandler(this, &MyForm::button_exit_Click);
			// 
			// button_for_next_game
			// 
			this->button_for_next_game->BackColor = System::Drawing::Color::MediumSlateBlue;
			this->button_for_next_game->BackgroundColor = System::Drawing::Color::MediumSlateBlue;
			this->button_for_next_game->BorderColor = System::Drawing::Color::PaleVioletRed;
			this->button_for_next_game->BorderRadius = 20;
			this->button_for_next_game->BorderSize = 0;
			this->button_for_next_game->FlatAppearance->BorderSize = 0;
			this->button_for_next_game->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->button_for_next_game->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->button_for_next_game->ForeColor = System::Drawing::Color::White;
			this->button_for_next_game->Location = System::Drawing::Point(126, 324);
			this->button_for_next_game->Name = L"button_for_next_game";
			this->button_for_next_game->Size = System::Drawing::Size(151, 51);
			this->button_for_next_game->TabIndex = 1;
			this->button_for_next_game->Text = L"New game";
			this->button_for_next_game->TextColor = System::Drawing::Color::White;
			this->button_for_next_game->UseVisualStyleBackColor = false;
			this->button_for_next_game->Click += gcnew System::EventHandler(this, &MyForm::button_for_next_game_Click);
			// 
			// tablo_winner
			// 
			this->tablo_winner->AutoSize = true;
			this->tablo_winner->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 21.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->tablo_winner->Location = System::Drawing::Point(84, 132);
			this->tablo_winner->Name = L"tablo_winner";
			this->tablo_winner->Size = System::Drawing::Size(139, 38);
			this->tablo_winner->TabIndex = 0;
			this->tablo_winner->Text = L"Winner is ";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Tempus Sans ITC", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(682, 23);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(92, 22);
			this->label4->TabIndex = 13;
			this->label4->Text = L"Player now:";
			this->label4->Visible = false;
			this->label4->Click += gcnew System::EventHandler(this, &MyForm::label4_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::PapayaWhip;
			this->ClientSize = System::Drawing::Size(1165, 745);
			this->Controls->Add(this->panel_end_of_game);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->panel1);
			this->ForeColor = System::Drawing::SystemColors::ControlText;
			this->Name = L"MyForm";
			this->Text = L"Hex_Game";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::MyForm_Paint);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel5->ResumeLayout(false);
			this->panel4->ResumeLayout(false);
			this->panel4->PerformLayout();
			this->panel3->ResumeLayout(false);
			this->panel3->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->panel_end_of_game->ResumeLayout(false);
			this->panel_end_of_game->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
	
	private: System::Void MyForm_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		
	}
	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label3_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label2_Click_1(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label4_Click(System::Object^ sender, System::EventArgs^ e) {
	}
		// don't touch
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void generation_Click_1(System::Object^ sender, System::EventArgs^ e) {
		setting_of_game local_setting = { this->size_of_field, this->computer, this->walks_first, this->method };
		generation_field(local_setting);
		this->panel1->Visible = false;
		this->tablo_player->Visible = true;
		this->tablo_player->Text = "";
		this->label4->Visible = true;
		if (this->walks_first == computer)
			this->computer_move();
	}
	private: System::Void panel_with_field_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
	}

	private: System::Void hex_Click_1(System::Object^ sender, System::EventArgs^ e) {
		int len_array = this -> size_of_field * this->size_of_field;
		int local_index = 0;
		bool we_find = false;

		for (; (local_index < len_array) && !we_find; ++local_index)
			we_find = sender->Equals(this->pictures[local_index]);
		--local_index;
		engine_of_game(local_index, main_controller.player_now());
	}
	private: void engine_of_game(int& index, const hex_gamer& player)
	{
		bool game_is_over = false;
		if (main_controller.check_step(index))
		{
			this->update_picture(index, player);
			this->tablo_player_TextChanged();
			if (main_controller.continue_game())
			{
				this->end_of_game();
				game_is_over = true;
			}
		}
		if (!game_is_over && (main_controller.player_now() == this->computer))
		{
			this->computer_move();
		}
	}

	private: void computer_move()
	{
		int move = monte_carlo_AI.simulation(main_controller);
		hex_gamer local_player = this->computer;
		this->engine_of_game(move, local_player);
	}

	private: void update_picture(const int& index, const hex_gamer& player_now)
	{
		if (player_now == second) {
			this->pictures[index]->ImageLocation = "hex_r.gif";
		}
		else if (player_now == first)
		{
			this->pictures[index]->ImageLocation = "hex_b.gif";
		}
		this->pictures[index]->Update();
	}
	private: void end_of_game()
	{
		this->label4->Visible = false;
		this->tablo_player->Visible = false;
		this->tablo_winner->Visible = true;
		this->panel_end_of_game->Visible = true;
		hex_gamer winner = main_controller.get_winner();
		this->print_winner(winner);
	}
	private: System::Void MinMax_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		this->method = minmax_method;
	}
	private: System::Void Monte_Carlo_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		this->method = monte_carlo_method;
	}
	private: System::Void Alpha_bata_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		this->method = alpha_beta_method;
	}
	private: System::Void but_first_pl_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		this->computer = first;
	}
	private: System::Void but_second_pl_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		this->computer = second;
	}
	private: System::Void but_none_pl_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		this->computer = none;
	}
	private: System::Void turn_first_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		this->walks_first = first;
	}
	private: System::Void second_turn_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
		this->walks_first = second;
	}
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		if (comboBox1->SelectedItem != nullptr) {
			this->size_of_field = Int32::Parse(comboBox1->Text);
		}
	}
	private: System::Void tablo_player_TextChanged() {
		if (main_controller.player_now() == first)
			this->tablo_player->Text = L"First";
		else
			this->tablo_player->Text = L"Second";
		this->tablo_player->Update();
	}
	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		if (main_controller.player_now() == first)
			this->tablo_player->Text = L"First";
		else
			this->tablo_player->Text = L"Second";
		this->tablo_player->Update();
	}
	private: System::Void print_winner(const hex_gamer& winner) {
		if (winner == first)
			this->tablo_winner->Text += L" first";
		else if (winner == second)
			this->tablo_winner->Text += L" second";
		this->tablo_winner->Update();
	}
	private: System::Void button_for_next_game_Click(System::Object^ sender, System::EventArgs^ e) {
		this->delete_field();
		this->panel1->Visible = true;
		this->but_first_pl_CheckedChanged(sender, e);
		this->Alpha_bata_CheckedChanged(sender, e);
		this->turn_first_CheckedChanged(sender, e);
	}
	private: System::Void button_exit_Click(System::Object^ sender, System::EventArgs^ e) {
		this->delete_field();
		Application::Exit();
	}
};
}