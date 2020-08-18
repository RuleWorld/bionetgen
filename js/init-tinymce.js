tinymce.init({
  /* editor area */
  selector: "textarea.editor",

  /* width and height */
  width: "100%",
  height: 500,

  /* status bar */
  statusbar: true,

  /* plugins */
  plugins : [
    "advlist autolink link image lists charmap print preview hr anchor pagebreak",
    "searchreplace wordcount visualblocks visualchars code fullscreen insertdatetime",
    "save table directionality emoticons template paste media nonbreaking"
  ],

  /* toolbar setup */
  toolbar: "insertfile undo redo | styleselect | bold italic | alignleft aligncenter alignright alignjustify | bulllist numlist outdent indent | link image | print preview media fullpage | forecolor backcolor emoticons",
})
