function addListener(element, eventName, handler) {
  if (element.addEventListener) {
    element.addEventListener(eventName, handler, false);
  }
  else if (element.attachEvent) {
    element.attachEvent('on' + eventName, handler);
  }
  else {
    element['on' + eventName] = handler;
  }
}

function next(elem) {
  do {
    elem = elem.nextSibling;
  } while (elem && elem.nodeType !== 1);
  return elem;
}

function onSectionClick(event) {
  var contentSection = next(this);
  if (contentSection) {
    // Add or remove css openSection class to show/hide section
    if (contentSection.className.match(/(?:^|\s)openSection(?!\S)/))
    {
      this.className = this.className.replace( /(?:^|\s)openSection(?!\S)/g , '' );
      contentSection.className = contentSection.className.replace( /(?:^|\s)openSection(?!\S)/g , '' );
    }
    else
    {
      this.className += " openSection";
      contentSection.className += " openSection";
    }
  }
}


function onPageLoaded() {
  var sections = document.getElementsByClassName("section_heading");
  [].forEach.call(sections, function(section) {
    addListener(section, 'click', onSectionClick);
  });
}

window.onload = onPageLoaded;
