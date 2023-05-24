let saveFile = () =>{
    const name = document.getElementById("item_name");
    const manufacturer = document.getElementById("item_manufacturer");
    const quantity = document.getElementById("item_quantity");
    const quality = document.getElementById("quality");
    const description = document.getElementById("description");
 
    var entry = "Name: " + name.value + "\n" + "Manufacturer: " + manufacturer.value + "\n";  
    entry += "Quantity: " + quantity.value + "\n" + "Quality: " + quality.value + "\n"; 
    entry += "Description: " + description.value + "\n";

    alert("SENT");
    var xhr = new XMLHttpRequest();

    xhr.open('POST', "127.0.0.1:8080", true);

    xhr.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
    
    xhr.send(entry);

};

/*
const name = document.getElementById("item_name");
    const manufacturer = document.getElementById("item_manufacturer");
    const quantity = document.getElementById("item_quantity");
    const quality = document.getElementById("quality");
    const description = document.getElementById("description");
 
    var entry = "Name: " + name.value + "\n" + "Manufacturer: " + manufacturer.value + "\n";  
    entry += "Quantity: " + quantity.value + "\n" + "Quality: " + quality.value + "\n"; 
    entry += "Description: " + description.value + "\n";
    */