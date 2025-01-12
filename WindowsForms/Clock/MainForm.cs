using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Clock
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			InitializeComponent();
			labelTime.BackColor = Color.AliceBlue;  // чтобы при изменении прозрачности lableTime отображалось лучше
			this.Location = new Point(Screen.PrimaryScreen.Bounds.Width - this.Width, 50);  // чтобы форма запускалась вправом верхнем углу, при этом Properties -> StartPosition -> Manual

		}
		
		 void SetVisibility(bool visible)
		{
			checkBoxShowDate.Visible = visible;
			checkBoxShowWeekDay.Visible = visible;
			buttonHideControls.Visible = visible;
			this.FormBorderStyle = visible?	FormBorderStyle.FixedDialog:FormBorderStyle.None;
			this.ShowInTaskbar = visible;
			this.TransparencyKey = visible?Color.Empty:this.BackColor; // второй делает прозрачным
		}
		private void timer_Tick(object sender, EventArgs e)
		{
			// обработчик события - это самая обычная функция, которая неявно вызывается при возникновении определенного события
			// у элемента интерфейса может быть множество событий и одно из них будет событием по умолчанию
			// для таймера событием по умолчанию является тик
			labelTime.Text = DateTime.Now.ToString("hh:mm:ss tt");  // , System.Globalization.CultureInfo.InvariantCulture); // если не включается
																	// labelTime.Text = DateTime.Now.ToString("HH:mm:ss");  // для 24-часового отображения
			if (checkBoxShowDate.Checked)
			{
				labelTime.Text += $"\n{DateTime.Now.ToString("yyyy.MM.dd")}";  // MM - большие это месяцы, mm - маленькие это минуты, при таком формате сортировка совпадает с сортировкой по имени, очень удобно использовать при резервном копировании

			}
			if (checkBoxShowWeekDay.Checked)
			{
				labelTime.Text += $"\n{DateTime.Now.DayOfWeek}";
			}
		}

		private void buttonHideControls_Click(object sender, EventArgs e)
		{
			SetVisibility(false);
		}

		private void labelTime_DoubleClick(object sender, EventArgs e)
		{
			SetVisibility(true);
		}
	}
}
