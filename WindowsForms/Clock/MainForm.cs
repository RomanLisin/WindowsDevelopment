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

		}
	}
}
