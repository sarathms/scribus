#include <Python.h>

#include "cmdobj.h"
#include "cmdutil.h"
#include "cmdvar.h"

PyObject *scribus_newrect(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	int i = Carrier->doc->ActPage->PaintRect(ValueToPoint(x), ValueToPoint(y),
															ValueToPoint(b), ValueToPoint(h),
															Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	Carrier->doc->ActPage->SetRectFrame(Carrier->doc->ActPage->Items.at(i));
	if (Name != "")
		Carrier->doc->ActPage->Items.at(i)->AnName = QString(Name);
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName);
}

PyObject *scribus_newellipse(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	int i = Carrier->doc->ActPage->PaintEllipse(ValueToPoint(x), ValueToPoint(y),
																 ValueToPoint(b), ValueToPoint(h),
																 Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	Carrier->doc->ActPage->SetOvalFrame(Carrier->doc->ActPage->Items.at(i));
	if (Name != "")
		Carrier->doc->ActPage->Items.at(i)->AnName = QString(Name);
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName);
}

PyObject *scribus_newimage(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	int i = Carrier->doc->ActPage->PaintPict(ValueToPoint(x), ValueToPoint(y), ValueToPoint(b), ValueToPoint(h));
	Carrier->doc->ActPage->SetRectFrame(Carrier->doc->ActPage->Items.at(i));
	if (Name != "")
		Carrier->doc->ActPage->Items.at(i)->AnName = QString(Name);
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName);
}

PyObject *scribus_newtext(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	int i = Carrier->doc->ActPage->PaintText(ValueToPoint(x), ValueToPoint(y),
															ValueToPoint(b), ValueToPoint(h),
															Carrier->doc->Dwidth, Carrier->doc->DpenText);
	Carrier->doc->ActPage->SetRectFrame(Carrier->doc->ActPage->Items.at(i));
	if (Name != "")
		Carrier->doc->ActPage->Items.at(i)->AnName = QString(Name);
	return PyString_FromString(Carrier->doc->ActPage->Items.at(i)->AnName);
}

PyObject *scribus_newline(PyObject *self, PyObject* args)
{
	double x, y, b, h;
	char *Name = "";
	if (!PyArg_ParseTuple(args, "dddd|s", &x, &y, &b, &h, &Name))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	x =	ValueToPoint(x);
	y = ValueToPoint(y);
	b = ValueToPoint(b);
	h = ValueToPoint(h);
	int i = Carrier->doc->ActPage->PaintPolyLine(x, y, 1, 1,	Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	PageItem *it = Carrier->doc->ActPage->Items.at(i);
	it->PoLine.resize(4);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	it->PoLine.setPoint(2, b-x, h-y);
	it->PoLine.setPoint(3, b-x, h-y);
	FPoint np2 = Carrier->doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
		{
		it->PoLine.translate(-np2.x(), 0);
		Carrier->doc->ActPage->MoveItem(np2.x(), 0, it);
		}
	if (np2.y() < 0)
		{
		it->PoLine.translate(0, -np2.y());
		Carrier->doc->ActPage->MoveItem(0, np2.y(), it);
		}
	Carrier->doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), i, false, false);
	Carrier->doc->ActPage->AdjustItemSize(it);
	if (Name != "")
		it->AnName = QString(Name);
	return PyString_FromString(it->AnName);
}

PyObject *scribus_polyline(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il;
	if ((!PyArg_ParseTuple(args, "O|s", &il, &Name)) || (!PyList_Check(il)))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	int len = PyList_Size(il);
	if ((len < 4) || ((len % 2) != 0))
		return PyString_FromString("");
	double x, y, b, h;
	int i = 0;
	x = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	int ic = Carrier->doc->ActPage->PaintPolyLine(x, y, 1, 1,	Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	PageItem *it = Carrier->doc->ActPage->Items.at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	int pp = 6;
	for (i = 2; i < len - 2; i += 2)
		{
		b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
		h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+1))));
		it->PoLine.resize(pp);
		it->PoLine.setPoint(pp-4, b-x, h-y);
		it->PoLine.setPoint(pp-3, b-x, h-y);
		it->PoLine.setPoint(pp-2, b-x, h-y);
		it->PoLine.setPoint(pp-1, b-x, h-y);
		pp += 4;
		}
	pp -= 2;
	b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-2))));
	h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-1))));
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-2, b-x, h-y);
	it->PoLine.setPoint(pp-1, b-x, h-y);
	FPoint np2 = Carrier->doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
		{
		it->PoLine.translate(-np2.x(), 0);
		Carrier->doc->ActPage->MoveItem(np2.x(), 0, it);
		}
	if (np2.y() < 0)
		{
		it->PoLine.translate(0, -np2.y());
		Carrier->doc->ActPage->MoveItem(0, np2.y(), it);
		}
	Carrier->doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false);
	Carrier->doc->ActPage->AdjustItemSize(it);
	if (Name != "")
		it->AnName = QString(Name);
	return PyString_FromString(it->AnName);
}

PyObject *scribus_polygon(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il;
	if ((!PyArg_ParseTuple(args, "O|s", &il, &Name)) || (!PyList_Check(il)))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	int len = PyList_Size(il);
	if ((len < 6) || ((len % 2) != 0))
		return PyString_FromString("");
	double x, y, b, h;
	int i = 0;
	x = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	int ic = Carrier->doc->ActPage->PaintPoly(x, y, 1, 1,	Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	PageItem *it = Carrier->doc->ActPage->Items.at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, 0, 0);
	int pp = 6;
	for (i = 2; i < len - 2; i += 2)
		{
		b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
		h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+1))));
		it->PoLine.resize(pp);
		it->PoLine.setPoint(pp-4, b-x, h-y);
		it->PoLine.setPoint(pp-3, b-x, h-y);
		it->PoLine.setPoint(pp-2, b-x, h-y);
		it->PoLine.setPoint(pp-1, b-x, h-y);
		pp += 4;
		}
	b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-2))));
	h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-1))));
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-4, b-x, h-y);
	it->PoLine.setPoint(pp-3, b-x, h-y);
	it->PoLine.setPoint(pp-2, b-x, h-y);
	it->PoLine.setPoint(pp-1, b-x, h-y);
	pp += 2;
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-2, 0, 0);
	it->PoLine.setPoint(pp-1, 0, 0);
	FPoint np2 = Carrier->doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
		{
		it->PoLine.translate(-np2.x(), 0);
		Carrier->doc->ActPage->MoveItem(np2.x(), 0, it);
		}
	if (np2.y() < 0)
		{
		it->PoLine.translate(0, -np2.y());
		Carrier->doc->ActPage->MoveItem(0, np2.y(), it);
		}
	Carrier->doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false);
	Carrier->doc->ActPage->AdjustItemSize(it);
	if (Name != "")
		it->AnName = QString(Name);
	return PyString_FromString(it->AnName);
}

PyObject *scribus_bezierline(PyObject *self, PyObject* args)
{
	char *Name = "";
	PyObject *il;
	if ((!PyArg_ParseTuple(args, "O|s", &il, &Name)) || (!PyList_Check(il)))
		return NULL;
	if (!Carrier->HaveDoc)
		return PyString_FromString("");
	int len = PyList_Size(il);
	if ((len < 8) || ((len % 6) != 0))
		return PyString_FromString("");
	double x, y, b, h, kx, ky, kx2, ky2;
	int i = 0;
	x = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	y = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	kx = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	ky = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	kx2 = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	ky2 = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
	i++;
	int ic = Carrier->doc->ActPage->PaintPolyLine(x, y, 1, 1,	Carrier->doc->Dwidth, Carrier->doc->Dbrush, Carrier->doc->Dpen);
	PageItem *it = Carrier->doc->ActPage->Items.at(ic);
	it->PoLine.resize(2);
	it->PoLine.setPoint(0, 0, 0);
	it->PoLine.setPoint(1, kx-x, ky-y);
	int pp = 6;
	for (i = 6; i < len - 6; i += 6)
		{
		b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i))));
		h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+1))));
		kx = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+2))));
		ky = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+3))));
		kx2 = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+4))));
		ky2 = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, i+5))));
		it->PoLine.resize(pp);
		it->PoLine.setPoint(pp-4, b-x, h-y);
		it->PoLine.setPoint(pp-3, kx-x, ky-y);
		it->PoLine.setPoint(pp-2, it->PoLine.point(pp-4));
		it->PoLine.setPoint(pp-1, kx2-x, ky2-y);
		pp += 4;
		}
	pp -= 2;
	b = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-6))));
	h = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-5))));
	kx = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-4))));
	ky = ValueToPoint(static_cast<double>(PyFloat_AsDouble(PyList_GetItem(il, len-3))));
	it->PoLine.resize(pp);
	it->PoLine.setPoint(pp-2, b-x, h-y);
	it->PoLine.setPoint(pp-1, kx-x, ky-y);
	FPoint np2 = Carrier->doc->ActPage->GetMinClipF(it->PoLine);
	if (np2.x() < 0)
		{
		it->PoLine.translate(-np2.x(), 0);
		Carrier->doc->ActPage->MoveItem(np2.x(), 0, it);
		}
	if (np2.y() < 0)
		{
		it->PoLine.translate(0, -np2.y());
		Carrier->doc->ActPage->MoveItem(0, np2.y(), it);
		}
	Carrier->doc->ActPage->SizeItem(it->PoLine.WidthHeight().x(), it->PoLine.WidthHeight().y(), ic, false, false);
	Carrier->doc->ActPage->AdjustItemSize(it);
	if (Name != "")
		it->AnName = QString(Name);
	return PyString_FromString(it->AnName);
}

PyObject *scribus_pathtext(PyObject *self, PyObject* args)
{
	double x, y;
	char *Name = "";
	char *TextB = "";
	char *PolyB = "";
	if (!PyArg_ParseTuple(args, "ddss|s", &x, &y, &TextB, &PolyB, &Name))
		return NULL;
	if ((!Carrier->HaveDoc) || ((TextB == "") || (PolyB == "")))
		return PyString_FromString("");
	int i = GetItem(QString(TextB));
	int ii = GetItem(QString(PolyB));
	if ((i == -1) || (ii == -1))
		return PyString_FromString("");
	Carrier->doc->ActPage->SelItem.clear();
	Carrier->doc->ActPage->SelItem.append(Carrier->doc->ActPage->Items.at(i));
	Carrier->doc->ActPage->SelItem.append(Carrier->doc->ActPage->Items.at(ii));
	PageItem *it = Carrier->doc->ActPage->Items.at(i);
	Carrier->doc->ActPage->ToPathText();
	Carrier->doc->ActPage->MoveItem(ValueToPoint(x) - it->Xpos, ValueToPoint(y) - it->Ypos, it);
	if (Name != "")
		it->AnName = QString(Name);
	return PyString_FromString(it->AnName);
}

/* 03/21/2004 - exception raised when Name doesn't exists. Doesn't crash then. (subik)
 */
PyObject *scribus_deleteobj(PyObject *self, PyObject* args)
{
	char *Name = "";
	if (!PyArg_ParseTuple(args, "|s", &Name))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;
	if (Name != "")
	{
		Carrier->doc->ActPage->SelItem.clear();
		int i = GetItem(QString(Name));
		if (i != -1)
			Carrier->doc->ActPage->SelItem.append(Carrier->doc->ActPage->Items.at(i));
		else
		{
			PyErr_SetString(PyExc_Exception, "Oook! You're trying to erase an object doesn't exist!");
			Py_DECREF(Py_None);
			return NULL;
		}
	}
	Carrier->doc->ActPage->DeleteItem();
	return Py_None;
}

/* 03/21/2004 - exception raises by non existent name (subik)
 */
PyObject *scribus_textflow(PyObject *self, PyObject* args)
{
	char* name;
	int id, state = -1;

	if (!PyArg_ParseTuple(args, "s|i", &name, &state))
		return NULL;
	Py_INCREF(Py_None);
	if (!Carrier->HaveDoc)
		return Py_None;

	id = GetItem(QString(name));
	if (id == -1)
	{
		PyErr_SetString(PyExc_Exception, "Oook! An object you're trying to textflow doesn't exist!");
		Py_DECREF(Py_None);
		return NULL;
	}

	if (state == -1)
	{
		Carrier->doc->ActPage->Items.at(id)->Textflow =
			!Carrier->doc->ActPage->Items.at(id)->Textflow;
	}
	else
	{
		state
			? Carrier->doc->ActPage->Items.at(id)->Textflow = true
			: Carrier->doc->ActPage->Items.at(id)->Textflow = false;
	} // if state null

	Carrier->view->DrawNew();
	Carrier->slotDocCh(true);
	return Py_None;
}
