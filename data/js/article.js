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

function closeSection(section) {
  section.className = section.className.replace( /(?:^|\s)openSection(?!\S)/g , '' );
  var contentSection = next(section);
  if (contentSection) {
    contentSection.className = contentSection.className.replace( /(?:^|\s)openSection(?!\S)/g , '' );
  }
}

function openSection(section) {
  var sections = document.getElementsByClassName("section_heading");
  [].forEach.call(sections, function(sectionToClose) {
    closeSection(sectionToClose);
  });
  section.className += " openSection";
  var contentSection = next(section);
  if (contentSection) {
    contentSection.className += " openSection";
  }
  window.history.replaceState({},"", window.location.href.split("#")[0] + '#' + section.id);
}

function onSectionClick(event) {
  if (this.className.match(/(?:^|\s)openSection(?!\S)/)) {
    closeSection(this);
  } else {
    openSection(this);
  }
}


function onPageLoaded() {
  var sections = document.getElementsByClassName("section_heading");
  [].forEach.call(sections, function(section) {
    addListener(section, 'click', onSectionClick);
  });

  var img = new Image();
  img.onload = function() {
    if (document.getElementById('articleImage').offsetWidth > this.width ) {
      var rad = document.getElementById('articleImage').offsetWidth / this.width;
      document.getElementById('articleImage').style.webkitFilter = "blur(" + rad + "px)";
      rad *= 2;
      document.getElementById('articleImage').style.margin = "-" + rad + "px -" + rad + "px -" + rad + "px -" + rad + "px";
    };
  }
  img.src = document.getElementById('articleImage').style.backgroundImage.replace(/url\((['"])?(.*?)\1\)/gi, '$2').split(',')[0];

}

window.onload = onPageLoaded;
