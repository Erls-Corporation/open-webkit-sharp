/*
 * Copyright (c) 2009, Peter Nelson (charn.opcode@gmail.com)
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, 
 *   this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice, 
 *   this list of conditions and the following disclaimer in the documentation 
 *   and/or other materials provided with the distribution.
 *   
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
*/

using System;
using System.Collections.Generic;
using System.Text;
using WebKit.Interop;

namespace WebKit.DOM
{
    /// <summary>
    /// Represents an HTML Document.
    /// </summary>
    public class HTMLDocument : Document
    {
        internal IDOMHTMLDocument doc;
        /// <summary>
        /// HTMLDocument constructor.
        /// </summary>
        /// <param name="Document">WebKit IDOMHTMLDocument object.</param>
        protected HTMLDocument(IDOMHTMLDocument Document)
            : base((IDOMDocument)Document)
        {
            doc = Document;
        }
        internal static HTMLDocument Create(IDOMHTMLDocument document)
        {
            return new HTMLDocument(document);
        }
        public string Cookie
        {
            get { return doc.cookie(); }
            set { doc.setCookie(value); }
        }
        public string Referrer
        {
            get { return doc.referrer(); }
        }
        public void Close()
        {
            doc.close();
        }
        public void Open()
        {
            doc.open();
        }
        public void Normalize()
        {
            doc.normalize();
        }
        public void Write(string text)
        {
            doc.Write(text);
        }
        public HTMLCollection Images
        {
            get { return new HTMLCollection(doc.images()); }
        }

        public HTMLCollection Anchors
        {
            get { return new HTMLCollection(doc.anchors()); }
        }

        public HTMLCollection Applets
        {
            get { return new HTMLCollection(doc.applets()); }
        }
        public HTMLCollection Links
        {
            get { return new HTMLCollection(doc.links()); }
        }
        public string Domain
        {
            get { return doc.Domain(); }
        }
        public string Title
        {
            get { return doc.title(); }
            set { doc.setTitle(value); }
        }
        public HTMLElement Body
        {
            get { return HTMLElement.Create((IDOMHTMLElement)doc.body()); }
        }
    }

    public class HTMLCollection
    {
        internal IDOMHTMLCollection collection;
        internal HTMLCollection(IDOMHTMLCollection coll)
        {
            collection = coll;
        }

        public Node GetItemAt(int index)
        {
            if (index > collection.length())
                throw new IndexOutOfRangeException();
            return Node.Create(collection.item(Convert.ToUInt32(index)));
        }

        public Node GetItemByName(string name)
        {
            return Node.Create(collection.namedItem(name));
        }

        public int Length
        {
            get { return Convert.ToInt32(collection.length()); }
        }
    }
    /// <summary>
    /// Represents an HTML Element
    /// </summary>
    public class HTMLElement : Element
    {
        private IDOMHTMLElement htmlElement;

        /// <summary>
        /// HTMLElement constructor.
        /// </summary>
        /// <param name="HTMLElement">WebKit IDOMHTMLElement object.</param>
        protected HTMLElement(IDOMHTMLElement HTMLElement)
            : base(HTMLElement)
        {
            this.htmlElement = HTMLElement;
        }

        internal static HTMLElement Create(IDOMHTMLElement HTMLElement)
        {
            return new HTMLElement(HTMLElement);
        }

        public string InnerHTML
        {
            get { return htmlElement.innerHTML(); }
            set { htmlElement.setInnerHTML(value); }
        }

        public string InnerText
        {
            get { return htmlElement.innerText(); }
            set { htmlElement.setInnerText(value); }
        }
    }
}
