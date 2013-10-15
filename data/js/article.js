// Launch our function when page loads
addListener(window, 'load', onPageLoaded);

// Initializes everything when page loads
function onPageLoaded() {
  // Restore src attribute to display images correctly
  restoreImgSrcAttr();

  // Install click handlers to open/close sections
  var sections = document.getElementsByTagName("h2");
  [].forEach.call(sections, function(section) {
    addListener(section, 'click', onSectionClick);
  });

  // Blur article heading images to hide resizing issues
  var img = new Image();
  img.onload = function() {
    if (document.getElementById('articleImage').offsetWidth > this.width ) {
      var rad = document.getElementById('articleImage').offsetWidth / this.width;
      document.getElementById('articleImage').style.webkitFilter = "blur(" + rad + "px)";
      rad *= 2;
      document.getElementById('articleImage').style.margin = "-" + rad + "px -" + rad + "px -" + rad + "px -" + rad + "px";
    };
  }
  var imgElement = document.getElementById('articleImage');
  // Avoid exceptions during debugging, when articles can be without any images
  if (imgElement)
    img.src = imgElement.style.backgroundImage.replace(/url\((['"])?(.*?)\1\)/gi, '$2').split(',')[0];
}

// For faster pages loading, we generate html with s attribute in img tag instead of src
// and dynamically replace it when the page loads
function restoreImgSrcAttr() {
  var elements = document.getElementsByTagName('img');
  for (var i = 0; i < elements.length; ++i) {
    var element = elements[i];
    var src = element.getAttribute('s');
    if (src != undefined) {
      element.setAttribute('src', src);
    }
  }
}

// Actions on section heading click
function onSectionClick(event) {
  var expand = !(this.className == "openSection");
  var contentSection = next(this);
  if (expand) {
    // Close opened sections
    var sections = document.getElementsByTagName("h2");
    [].forEach.call(sections, function(sectionToClose) {
      sectionToClose.className = next(sectionToClose).className = "";
    });

    this.className = contentSection.className = "openSection";

    animateScroll(this, 200);

  } else {
    this.className = contentSection.className = "";
  }
}

// Helper to add handlers
function addListener(element, eventName, handler) {
  if (element.addEventListener)
    element.addEventListener(eventName, handler, false);
  else if (element.attachEvent)
    element.attachEvent('on' + eventName, handler);
  else
    element['on' + eventName] = handler;
}

// Get next sibling
function next(elem) {
  do {
    elem = elem.nextSibling;
  } while (elem && elem.nodeType !== 1);
  return elem;
}

/******************************************************************/
var ANIMATION_INTERVAL = 10; // milliseconds

// Scroll section heading to the top
function animateScroll(element, duration) {
  if (duration < 0)
    return;
  var yLeft = element.getBoundingClientRect().top;
  var dy = yLeft / duration * ANIMATION_INTERVAL;
  window.scrollBy(0, dy);

  setTimeout(function() {
    animateScroll(element, duration - ANIMATION_INTERVAL);
  }, ANIMATION_INTERVAL);
}
