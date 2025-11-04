import os
from pygments import highlight
from pygments.lexers import CppLexer
from pygments.formatters import HtmlFormatter
from reportlab.lib.pagesizes import A4
from reportlab.pdfgen import canvas
from reportlab.lib.units import inch
from reportlab.platypus import Paragraph, SimpleDocTemplate, PageBreak
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.platypus import SimpleDocTemplate, Paragraph, PageBreak, Preformatted
from reportlab.lib.enums import TA_LEFT
from bs4 import BeautifulSoup
from copy import deepcopy
import html

folder = "../"
output_pdf = "merged_cpp_files.pdf"
cpp_files = [f for f in os.listdir(folder) if f.endswith(".cpp")]
print(cpp_files)

class CounterDoc(SimpleDocTemplate):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self._count = 0
    def afterPage(self):
        super().afterPage()
        self._count += 1

styles = getSampleStyleSheet()
styles.add(ParagraphStyle(
    name="MyCode",
    fontName="Courier",
    fontSize=9,
    leading=10,
    alignment=TA_LEFT
))

files = []
for filename in cpp_files:
    story = [Paragraph(f"<b>{filename}</b>", styles["Heading2"])]
    with open(f"{folder}{filename}", "r", encoding="utf-8") as f:
        code = f.read().expandtabs(4)

    story.append(Preformatted(code, styles["MyCode"]))
    story.append(PageBreak())
    
    _story = deepcopy(story)
    doc = CounterDoc(output_pdf, pagesize=A4)
    doc.build(_story)
    files.append((story, doc._count))

def add_page_number(canvas, doc):
    page_num = canvas.getPageNumber()
    text = f"{page_num}"
    canvas.saveState()
    canvas.setFont("Helvetica", 9)
    canvas.drawCentredString(A4[0] / 2.0, 0.5 * inch, text)
    canvas.restoreState()

files.sort(key=lambda x: x[1] % 2)
story = []
for _story, _len in files:
    story.extend(_story)
doc = SimpleDocTemplate(output_pdf, pagesize=A4)
doc.build(story, onFirstPage=add_page_number, onLaterPages=add_page_number)
print(f"Created {output_pdf}")